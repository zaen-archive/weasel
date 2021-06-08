#include <map>
#include <memory>
#include <string>
#include <vector>

#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Verifier.h"
#include "llvm/ADT/STLExtras.h"

// LEXER
enum TokenKind
{
    eof = -1,
    key_fun = -2,
    key_extern = -3,
    identifier = -4,
    number = -5
};

// Token Value
std::string _identifierStr;
double _numVal;
int _currentToken;
int _lastChar = ' ';

// LLVM Config
llvm::LLVMContext _context;
llvm::IRBuilder<> _builder(_context);
std::unique_ptr<llvm::Module> _module;
std::map<std::string, llvm::Value *> _namedValues;

// Log Error
void logError(std::string msg)
{
    fprintf(stderr, "Error : %s\n", msg.c_str());
}

int getTokenKind()
{
    while (isspace(_lastChar))
    {
        _lastChar = getchar();
    }

    // Alphanum character [A-Za-z][a-zA-Z0-9]
    while (isalpha(_lastChar))
    {
        _identifierStr = _lastChar;
        while (isalnum(_lastChar = getchar()))
        {
            _identifierStr += _lastChar;
        }

        if (_identifierStr == "fun")
        {
            return TokenKind::key_fun;
        }

        if (_identifierStr == "extern")
        {
            return TokenKind::key_extern;
        }

        return TokenKind::identifier;
    }

    // Digit Character [0-9]
    while (std::isdigit(_lastChar))
    {
        std::string numStr;

        do
        {
            numStr += _lastChar;
            _lastChar = getchar();
        } while (std::isdigit(_lastChar) || _lastChar == '.');

        _numVal = std::strtod(numStr.c_str(), 0);
        return TokenKind::number;
    }

    // Comment Character //
    if (_lastChar == '/')
    {
        if ((_lastChar = getchar()) == '/')
        {
            logError("Expecting /, but get " + _lastChar);
        }

        do
        {
            _lastChar = getchar();
        } while (_lastChar != EOF && _lastChar != '\n' && _lastChar != '\r');

        if (_lastChar != EOF)
        {
            return getTokenKind();
        }
    }

    int thisChar = _lastChar;
    _lastChar = getchar(); // eat last char
    return thisChar;
}

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

/// Base Class For All Expression Node
class ExprAST
{
public:
    virtual llvm::Value *codegen() = 0;
};

// NumberExprAST - Expression for number
class NumberExprAST : public ExprAST
{
    double _val;

public:
    NumberExprAST(double val) : _val(val) {}
    llvm::Value *codegen();
};

llvm::Value *NumberExprAST::codegen()
{
    return llvm::ConstantFP::get(_context, llvm::APFloat(this->_val));
}

// VariableExprAST - Expression for variable
class VariableExprAST : public ExprAST
{
    std::string _name;

public:
    VariableExprAST(std::string &name) : _name(name) {}
    llvm::Value *codegen();
};

llvm::Value *VariableExprAST::codegen()
{
    auto *v = _namedValues[_name];
    if (!v)
    {
        logError("Unknown variable name " + this->_name);
        return nullptr;
    }

    return v;
}

// BinaryExprAST - Expression for binary expression
class BinaryExprAST : public ExprAST
{
    char _op;
    std::unique_ptr<ExprAST> _lhs, _rhs;

public:
    BinaryExprAST(
        char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs) : _op(op), _lhs(std::move(lhs)), _rhs(std::move(rhs)) {}
    llvm::Value *codegen();
};

llvm::Value *BinaryExprAST::codegen()
{
    auto *l = _lhs->codegen();
    auto *r = _rhs->codegen();
    if (!l || !r)
    {
        return nullptr;
    }

    switch (_op)
    {
    case '+':
        return _builder.CreateFAdd(l, r, "addtmp");
    case '-':
        return _builder.CreateFSub(l, r, "subtmp");
    case '*':
        return _builder.CreateFMul(l, r, "multmp");
    case '/':
        return _builder.CreateFDiv(l, r, "divtmp");
    case '<':
        l = _builder.CreateFCmpULT(l, r, "cmptmp");
        return _builder.CreateUIToFP(l, llvm::Type::getDoubleTy(_context), "booltmp");
    case '>':
        l = _builder.CreateFCmpUGT(l, r, "cmptmp");
        return _builder.CreateUIToFP(l, llvm::Type::getDoubleTy(_context), "booltmp");
    default:
        logError("Invalid binary operator");
        return nullptr;
    }
}

// CallExprAST - Expression for Function Call
class CallExprAST : public ExprAST
{
    std::string _callee;
    std::vector<std::unique_ptr<ExprAST>> _args;

public:
    CallExprAST(
        std::string &callee, std::vector<std::unique_ptr<ExprAST>> args) : _callee(callee), _args(std::move(args)) {}
    llvm::Value *codegen();
};

llvm::Value *CallExprAST::codegen()
{
    auto *f = _module->getFunction(_callee);
    if (!f)
    {
        logError("Unknown function referenced");
        return nullptr;
    }

    if (f->arg_size() != _args.size())
    {
        logError("Incorrect number of arguments passed");
        return nullptr;
    }

    std::vector<llvm::Value *> argsV;
    for (size_t i = 0; i < _args.size(); i++)
    {
        argsV.push_back(_args[i]->codegen());
        if (!argsV.back())
        {
            return nullptr;
        }
    }

    return _builder.CreateCall(f, argsV, "calltmp");
}

// PrototyeAST - Prototype for function (DECLARATION)
class PrototypeAST
{
    std::string _name;
    std::vector<std::string> _args;

public:
    PrototypeAST(
        const std::string &name, std::vector<std::string> args) : _name(name), _args(std::move(args)) {}
    llvm::Function *codegen();
    std::string getName() { return _name; }
};

llvm::Function *PrototypeAST::codegen()
{
    std::vector<llvm::Type *> doubles(_args.size(), llvm::Type::getDoubleTy(_context));
    auto *fTy = llvm::FunctionType::get(llvm::Type::getDoubleTy(_context), doubles, false);
    auto *f = llvm::Function::Create(fTy, llvm::Function::ExternalLinkage, _name, _module.get());

    int idx = 0;
    for (auto &arg : f->args())
    {
        arg.setName(_args[idx++]);
    }

    return f;
}

// FunctionAST - Function Definition AST
class FunctionAST
{
    std::unique_ptr<PrototypeAST> _proto;
    std::unique_ptr<ExprAST> _body;

public:
    FunctionAST(
        std::unique_ptr<PrototypeAST> proto,
        std::unique_ptr<ExprAST> body) : _proto(std::move(proto)), _body(std::move(body)) {}
    llvm::Function *codegen();
};

llvm::Function *FunctionAST::codegen()
{
    auto *f = _module->getFunction(_proto->getName());
    if (!f)
    {
        f = _proto->codegen();
    }

    if (!f)
    {
        return nullptr;
    }

    if (!f->empty())
    {
        logError("Function cannot be redefined.");
        return nullptr;
    }

    // Create new Basic Block
    auto *bb = llvm::BasicBlock::Create(_context, "entry", f);
    _builder.SetInsertPoint(bb);

    // Record the Function arguments in named values
    _namedValues.clear();
    for (auto &arg : f->args())
    {
        _namedValues[std::string(arg.getName())] = &arg;
    }

    if (auto *retVal = _body->codegen())
    {
        // Finish off the function
        _builder.CreateRet(retVal);

        // Validate the generated code, checking for consistency
        llvm::verifyFunction(*f);

        return f;
    }

    // Error reading body, remove function.
    f->eraseFromParent();
    return nullptr;
}

// Declare Parsing Functions //
std::unique_ptr<ExprAST> parseExpression();
std::unique_ptr<ExprAST> parseBinOpRHS(int prec, std::unique_ptr<ExprAST> lhs);

// getNextToken - Get Next Token
int getNextToken()
{
    return _currentToken = getTokenKind();
}

// Parsing Number
std::unique_ptr<ExprAST> parseNumberExpr()
{
    auto result = std::make_unique<NumberExprAST>(_numVal);
    getNextToken();

    return std::move(result);
}

// Parsing Paren Expression
std::unique_ptr<ExprAST> parseParenExpr()
{
    getNextToken(); // eat (

    auto v = parseExpression();
    if (!v)
    {
        return nullptr;
    }

    if (_currentToken != ')')
    {
        logError("Expected ')'");
        return nullptr;
    }

    getNextToken();

    return v;
}

std::unique_ptr<ExprAST> parseIdentifierExpr()
{
    std::string name = _identifierStr;

    getNextToken(); // eat identifier

    // Simple Variable
    if (_currentToken != '(')
    {
        return std::make_unique<VariableExprAST>(name);
    }

    getNextToken(); // eat (
    std::vector<std::unique_ptr<ExprAST>> args;
    if (_currentToken != ')')
    {
        while (true)
        {
            if (auto arg = parseExpression())
            {
                args.push_back(std::move(arg));
            }
            else
            {
                return nullptr;
            }

            if (_currentToken == ')')
            {
                break;
            }

            if (_currentToken != ',')
            {
                logError("Expected ')' or ',' in argument list");
                return nullptr;
            }

            getNextToken();
        }
    }

    // eat ')'
    getNextToken();

    return std::make_unique<CallExprAST>(name, std::move(args));
}

std::unique_ptr<ExprAST> parsePrimary()
{
    switch (_currentToken)
    {
    case TokenKind::identifier:
        return parseIdentifierExpr();
    case TokenKind::number:
        return parseNumberExpr();
    case '(':
        return parseParenExpr();
    default:
        std::string str;
        str.push_back(_currentToken);
        logError("unknown token (" + str + " -> " + std::to_string(_currentToken) + ") when expecting an expression");
        return nullptr;
    }
}

//Get Token Precedence of pending binary operator
int getTokenPrecedence()
{
    if (!isascii(_currentToken))
        return -1;

    switch (_currentToken)
    {
    case '<':
    case '>':
        return 10;
    case '-':
    case '+':
        return 20;
    case '/':
    case '*':
        return 40;
    default:
        return -1;
    }
}

// Parsing Expression
std::unique_ptr<ExprAST> parseExpression()
{
    auto lhs = parsePrimary();
    if (!lhs)
        return nullptr;

    return parseBinOpRHS(0, std::move(lhs));
}

// binoprhs
//   ::= ('+' primary)*
std::unique_ptr<ExprAST> parseBinOpRHS(int prec, std::unique_ptr<ExprAST> lhs)
{
    //If this is a binop, find it's preccedence
    while (true)
    {
        int tokPrec = getTokenPrecedence();

        //consume it
        if (tokPrec < prec)
            return lhs;

        //BinOp
        int binOp = _currentToken;
        getNextToken(); // eaat binop

        // Parse the primary expression after the binary operator
        auto rhs = parsePrimary();
        if (!rhs)
        {
            return nullptr;
        }

        // if BinOp binds lesss tightly with rhs than the operator after rhs
        //the pending operator take rhs as its lhs
        int nextPrec = getTokenPrecedence();
        if (tokPrec < nextPrec)
        {
            rhs = parseBinOpRHS(tokPrec + 1, std::move(rhs));
            if (!rhs)
            {
                return nullptr;
            }
        }

        //Merge lhs/rhs
        lhs = std::make_unique<BinaryExprAST>(binOp, std::move(lhs), std::move(rhs));
    }
}

/// prototype
///   ::= id '(' id* ')'
std::unique_ptr<PrototypeAST> parsePrototype()
{
    if (_currentToken != TokenKind::identifier)
    {
        logError("Expected function name in prototype");
        return nullptr;
    }

    std::string fnName = _identifierStr;
    getNextToken();

    if (_currentToken != '(')
    {
        logError("Expected '(' in prototype");
        return nullptr;
    }

    //Read List of argument
    std::vector<std::string> args;
    while (getNextToken() == TokenKind::identifier)
    {
        args.push_back(_identifierStr);
    }

    if (_currentToken != ')')
    {
        logError("Expected ')' in prototype, found " + std::to_string(_currentToken));
        return nullptr;
    }

    //success
    getNextToken(); // eat ')'

    return std::make_unique<PrototypeAST>(fnName, std::move(args));
}

/// definition ::= 'fun' prototype expression
std::unique_ptr<FunctionAST> parseDefinition()
{
    getNextToken(); // eat fun

    auto proto = parsePrototype();
    if (!proto)
    {
        return nullptr;
    }

    auto expr = parseExpression();
    if (!expr)
    {
        return nullptr;
    }

    return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
}

/// external ::= 'extern' prototype
std::unique_ptr<PrototypeAST> parseExtern()
{
    getNextToken(); // eat extern

    return parsePrototype();
}

/// toplevelexpr ::= expression
std::unique_ptr<FunctionAST> parseTopLevelExpr()
{
    auto expr = parseExpression();
    if (!expr)
    {
        return nullptr;
    }

    auto proto = std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string>());
    return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
}

//===----------------------------------------------------------------------===//
// Top-Level parsing and JIT Driver
//===----------------------------------------------------------------------===//

static void HandleDefinition()
{
    if (auto FnAST = parseDefinition())
    {
        if (auto *FnIR = FnAST->codegen())
        {
            fprintf(stderr, "\nRead function definition:");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    }
    else
    {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleExtern()
{
    if (auto ProtoAST = parseExtern())
    {
        if (auto *FnIR = ProtoAST->codegen())
        {
            fprintf(stderr, "Read extern: ");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    }
    else
    {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleTopLevelExpression()
{
    // Evaluate a top-level expression into an anonymous function.
    if (auto FnAST = parseTopLevelExpr())
    {
        if (auto *FnIR = FnAST->codegen())
        {
            fprintf(stderr, "Read top-level expression:");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    }
    else
    {
        // Skip token for error recovery.
        getNextToken();
    }
}

// Main Loop
/// top ::= definition | external | expression | ';'
void mainLoop()
{
    while (1)
    {
        fprintf(stderr, "ready > ");

        switch (_currentToken)
        {
        case TokenKind::eof:
            return;
        case ';': // ignore top-level semicolons.
            getNextToken();
            break;
        case TokenKind::key_fun:
            HandleDefinition();
            break;
        case TokenKind::key_extern:
            HandleExtern();
            break;
        default:
            HandleTopLevelExpression();
            break;
        }
    }
}

int main()
{
    fprintf(stderr, "ready > ");
    getNextToken();

    _module = std::make_unique<llvm::Module>("The JIT", _context);

    mainLoop();

    _module->print(llvm::errs(), nullptr);
}
