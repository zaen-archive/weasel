// #include <map>
// #include <memory>
// #include <string>
// #include <vector>

// #include "llvm/IR/Value.h"
// #include "llvm/IR/IRBuilder.h"
// #include "llvm/IR/Module.h"
// #include "llvm/IR/Constant.h"
// #include "llvm/IR/Verifier.h"
// #include "llvm/IR/PassManager.h"
// #include "llvm/IR/PassManagerImpl.h"
// #include "llvm/IR/PassManagerInternal.h"
// #include "llvm/IR/LegacyPassManager.h"
// #include "llvm/IR/Function.h"
// #include "llvm/Transforms/InstCombine/InstCombine.h"
// #include "llvm/Transforms/Scalar.h"
// #include "llvm/Transforms/Scalar/GVN.h"
// #include "llvm/Target/TargetMachine.h"
// #include "llvm/Support/TargetSelect.h"
// #include "KaleidoscopeJIT.h"

// // LEXER
// enum TokenKind {
//     eof = -1,
//     key_fun = -2,
//     key_extern = -3,
//     identifier = -4,
//     number = -5
// };

// // Token Value
// std::string _identifierStr;
// double _numVal;
// int _currentToken;
// int _lastChar = ' ';


// // LLVM Config
// llvm::LLVMContext _context;
// llvm::IRBuilder<> _builder(_context);
// std::unique_ptr<llvm::Module> _module;
// std::map<std::string, llvm::Value *> _namedValues;

// // Function Pass Manager
// // Optimization
// std::unique_ptr<llvm::legacy::FunctionPassManager> _fpm;

// // kaleidoscope JIT
// std::unique_ptr<llvm::orc::KaleidoscopeJIT> _jit;

// // Initialize Function Manager
// void initializeModuleAndPassManager() {
//     _module = std::make_unique<llvm::Module>("The JIT", _context);
//     _module->setDataLayout(_jit->getTargetMachine().createDataLayout());

//     // Create New Pass Manager
//     _fpm = std::make_unique<llvm::legacy::FunctionPassManager>(_module.get());

//     // Do Simple "peephole" and bit-twiddling optimazation
//     _fpm->add(llvm::createInstructionCombiningPass());

//     // Reassociate expression
//     _fpm->add(llvm::createReassociatePass());

//     // Eliminate common SubExpression
//     _fpm->add(llvm::createGVNPass());

//     // Simplify the control flow graph (deleting unreachable block)
//     _fpm->add(llvm::createCFGSimplificationPass());

//     // Initialize
//     _fpm->doInitialization();
// }

// // Log Error
// void logError(std::string msg) {
//     fprintf(stderr, "Error : %s\n", msg.c_str());
// }

// int getTokenKind() {
//     while (isspace(_lastChar))
//     {
//         _lastChar = getchar();
//     }
    
//     // Alphanum character [A-Za-z][a-zA-Z0-9]
//     while (isalpha(_lastChar))
//     {
//         _identifierStr = _lastChar;
//         while (isalnum(_lastChar = getchar()))
//         {
//             _identifierStr += _lastChar;
//         }
        
//         if (_identifierStr == "fun") {
//             return TokenKind::key_fun;
//         }

//         if (_identifierStr == "extern") {
//             return TokenKind::key_extern;
//         }

//         return TokenKind::identifier;
//     }
    
//     // Digit Character [0-9]
//     while (std::isdigit(_lastChar))
//     {
//         std::string numStr;

//         do {
//             numStr += _lastChar;
//             _lastChar = getchar();
//         } while (std::isdigit(_lastChar) || _lastChar == '.');

//         _numVal = std::strtod(numStr.c_str(), 0);
//         return TokenKind::number;
//     }

//     // Comment Character //
//     if (_lastChar == '/') {
//         if ((_lastChar = getchar()) == '/') {
//             logError("Expecting /, but get " + _lastChar);
//         }
        
//         do {
//             _lastChar = getchar();
//         } while (_lastChar != EOF && _lastChar != '\n' && _lastChar != '\r');
        
//         if (_lastChar != EOF) {
//             return getTokenKind();
//         }
//     }

//     int thisChar = _lastChar;
//     _lastChar = getchar(); // eat last char
//     return thisChar;
// }

// //===----------------------------------------------------------------------===//
// // Abstract Syntax Tree (aka Parse Tree)
// //===----------------------------------------------------------------------===//

// /// Base Class For All Expression Node
// class ExprAST
// {
// public:
//     virtual llvm::Value *codegen() = 0;
// };

// // NumberExprAST - Expression for number
// class NumberExprAST : public ExprAST
// {
//     double _val;

// public:
//     NumberExprAST(double val) : _val(val) {}
//     llvm::Value *codegen();
// };

// llvm::Value *NumberExprAST::codegen() {
//     return llvm::ConstantFP::get(_context, llvm::APFloat(this->_val));
// }

// // VariableExprAST - Expression for variable
// class VariableExprAST : public ExprAST
// {
//     std::string _name;
// public:
//     VariableExprAST(std::string &name) : _name(name) {}
//     llvm::Value *codegen();
// };

// llvm::Value *VariableExprAST::codegen() {
//     auto *v = _namedValues[_name];
//     if (!v) {
//         logError("Unknown variable name " + this->_name);
//         return nullptr;
//     }

//     return v;
// }

// // BinaryExprAST - Expression for binary expression
// class BinaryExprAST : public ExprAST
// {
//     char _op;
//     std::unique_ptr<ExprAST> _lhs, _rhs;
// public:
//     BinaryExprAST(
//         char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs
//     ) : _op(op), _lhs(std::move(lhs)), _rhs(std::move(rhs)) {}
//     llvm::Value *codegen();
// };

// llvm::Value *BinaryExprAST::codegen() {
//     auto *l = _lhs->codegen();
//     auto *r = _rhs->codegen();
//     if (!l || !r) {
//         return nullptr;
//     }

//     switch (_op)
//     {
//     case '+':
//         return _builder.CreateFAdd(l, r, "addtmp");
//     case '-':
//         return _builder.CreateFSub(l, r, "subtmp");
//     case '*':
//         return _builder.CreateFMul(l, r, "multmp");
//     case '/':
//         return _builder.CreateFDiv(l, r, "divtmp");
//     case '<':
//         l = _builder.CreateFCmpULT(l, r, "cmptmp");
//         return _builder.CreateUIToFP(l, llvm::Type::getDoubleTy(_context), "booltmp");
//     case '>':
//         l = _builder.CreateFCmpUGT(l, r, "cmptmp");
//         return _builder.CreateUIToFP(l, llvm::Type::getDoubleTy(_context), "booltmp");
//     default:
//         logError("Invalid binary operator");
//         return nullptr;
//     }
// }

// // CallExprAST - Expression for Function Call
// class CallExprAST : public ExprAST
// {
//     std::string _callee;
//     std::vector<std::unique_ptr<ExprAST>> _args;
// public:
//     CallExprAST(
//         std::string &callee, std::vector<std::unique_ptr<ExprAST>> args
//     ) : _callee(callee), _args(std::move(args)) {}
//     llvm::Value *codegen();
// };

// llvm::Value *CallExprAST::codegen() {
//     auto *f = _module->getFunction(_callee);
//     if (!f) {
//         logError("Unknown function referenced");
//         return nullptr;
//     }

//     if (f->arg_size() != _args.size()) {
//         logError("Incorrect number of arguments passed");
//         return nullptr;
//     }

//     std::vector<llvm::Value *> argsV;
//     for (size_t i = 0; i < _args.size(); i++)
//     {
//         argsV.push_back(_args[i]->codegen());
//         if (!argsV.back()) {
//             return nullptr;
//         }
//     }
    
//     return _builder.CreateCall(f, argsV, "calltmp");
// }

// // PrototyeAST - Prototype for function (DECLARATION)
// class PrototypeAST
// {
//     std::string _name;
//     std::vector<std::string> _args;
// public:
//     PrototypeAST(
//         const std::string &name, std::vector<std::string> args
//     ) : _name(name), _args(std::move(args)) {}
//     llvm::Function *codegen();
//     std::string getName() { return _name; }
// };

// llvm::Function *PrototypeAST::codegen() {
//     std::vector<llvm::Type *> doubles(_args.size(), llvm::Type::getDoubleTy(_context));
//     auto *fTy = llvm::FunctionType::get(llvm::Type::getDoubleTy(_context), doubles, false);
//     auto *f = llvm::Function::Create(fTy, llvm::Function::ExternalLinkage, _name, _module.get());

//     int idx = 0;
//     for (auto &arg: f->args()) {
//         arg.setName(_args[idx++]);
//     }

//     return f;
// }

// // FunctionAST - Function Definition AST
// class FunctionAST
// {
//     std::unique_ptr<PrototypeAST> _proto;
//     std::unique_ptr<ExprAST> _body;
// public:
//     FunctionAST(
//         std::unique_ptr<PrototypeAST> proto,
//         std::unique_ptr<ExprAST> body
//     ) : _proto(std::move(proto)), _body(std::move(body)) {}
//     llvm::Function *codegen();
// };

// llvm::Function *FunctionAST::codegen() {
//     auto *f = _module->getFunction(_proto->getName());
//     if (!f) {
//         f = _proto->codegen();
//     }

//     if (!f) {
//         return nullptr;
//     }

//     if (!f->empty()) {
//         logError("Function cannot be redefined.");
//         return nullptr;
//     }

//     // Create new Basic Block
//     auto *bb = llvm::BasicBlock::Create(_context, "entry", f);
//     _builder.SetInsertPoint(bb);

//     // Record the Function arguments in named values
//     _namedValues.clear();
//     for (auto &arg: f->args()) {
//         _namedValues[std::string(arg.getName())] = &arg;
//     }

//     if (auto *retVal = _body->codegen()) {
//         // Finish off the function
//         _builder.CreateRet(retVal);

//         // Validate the generated code, checking for consistency
//         llvm::verifyFunction(*f);

//         // Optimize the Function
//         _fpm->run(*f);

//         return f;
//     }

//     // Error reading body, remove function.
//     f->eraseFromParent();
//     return nullptr;
// }

// // Declare Parsing Functions //
// std::unique_ptr<ExprAST> parseExpression();
// std::unique_ptr<ExprAST> parseBinOpRHS(int prec, std::unique_ptr<ExprAST> lhs);

// // getNextToken - Get Next Token
// int getNextToken() {
//     return _currentToken = getTokenKind();
// }

// // Parsing Number
// std::unique_ptr<ExprAST> parseNumberExpr() {
//     auto result = std::make_unique<NumberExprAST>(_numVal);
//     getNextToken();

//     return std::move(result);
// }

// // Parsing Paren Expression
// std::unique_ptr<ExprAST> parseParenExpr() {
//     getNextToken(); // eat (
    
//     auto v = parseExpression();
//     if (!v) {
//         return nullptr;
//     }

//     if (_currentToken != ')') {
//         logError("Expected ')'");
//         return nullptr;
//     }

//     getNextToken();

//     return v;
// }

// std::unique_ptr<ExprAST> parseIdentifierExpr() {
//     std::string name = _identifierStr;

//     getNextToken(); // eat identifier

//     // Simple Variable
//     if (_currentToken != '(') {
//         return std::make_unique<VariableExprAST>(name);
//     }

//     getNextToken(); // eat (
//     std::vector<std::unique_ptr<ExprAST>> args;
//     if (_currentToken != ')') {
//         while (true)
//         {
//             if (auto arg = parseExpression()) {
//                 args.push_back(std::move(arg));
//             } else {
//                 return nullptr;
//             }

//             if (_currentToken == ')') {
//                 break;
//             }

//             if (_currentToken != ',') {
//                 logError("Expected ')' or ',' in argument list");
//                 return nullptr;
//             }

//             getNextToken();
//         }
//     }

//     // eat ')'
//     getNextToken();

//     return std::make_unique<CallExprAST>(name, std::move(args));
// }

// std::unique_ptr<ExprAST> parsePrimary() {
//     switch (_currentToken)
//     {
//     case TokenKind::identifier: return parseIdentifierExpr();
//     case TokenKind::number: return parseNumberExpr();
//     case '(': return parseParenExpr();
//     default:
//         std::string str;
//         str.push_back(_currentToken);
//         logError("unknown token (" + str + " -> " + std::to_string(_currentToken) + ") when expecting an expression");
//         return nullptr;
//     }
// }

// //Get Token Precedence of pending binary operator
// int getTokenPrecedence() {
//     if (!isascii(_currentToken)) return -1;

//     switch (_currentToken)
//     {
//     case '<':
//     case '>': return 10;
//     case '-':
//     case '+': return 20;
//     case '/':
//     case '*': return 40;
//     default: return -1;
//     }
// }

// // Parsing Expression
// std::unique_ptr<ExprAST> parseExpression() {
//     auto lhs = parsePrimary();
//     if (!lhs) return nullptr;

//     return parseBinOpRHS(0, std::move(lhs));
// }

// // binoprhs
// //   ::= ('+' primary)*
// std::unique_ptr<ExprAST> parseBinOpRHS(int prec, std::unique_ptr<ExprAST> lhs) {
//     //If this is a binop, find it's preccedence
//     while (true)
//     {
//         int tokPrec = getTokenPrecedence();

//         //consume it
//         if (tokPrec < prec) return lhs;

//         //BinOp
//         int binOp = _currentToken;
//         getNextToken(); // eaat binop

//         // Parse the primary expression after the binary operator
//         auto rhs = parsePrimary();
//         if (!rhs) {
//             return nullptr;
//         }

//         // if BinOp binds lesss tightly with rhs than the operator after rhs
//         //the pending operator take rhs as its lhs
//         int nextPrec = getTokenPrecedence();
//         if (tokPrec < nextPrec) {
//             rhs = parseBinOpRHS(tokPrec + 1, std::move(rhs));
//             if (!rhs) {
//                 return nullptr;
//             }
//         }

//         //Merge lhs/rhs
//         lhs = std::make_unique<BinaryExprAST>(binOp, std::move(lhs), std::move(rhs));
//     }
// }

// /// prototype
// ///   ::= id '(' id* ')'
// std::unique_ptr<PrototypeAST> parsePrototype() {
//     if (_currentToken != TokenKind::identifier) {
//         logError("Expected function name in prototype");
//         return nullptr;
//     }

//     std::string fnName = _identifierStr;
//     getNextToken();

//     if (_currentToken != '(') {
//         logError("Expected '(' in prototype");
//         return nullptr;
//     }

//     //Read List of argument
//     std::vector<std::string> args;
//     while (getNextToken() == TokenKind::identifier)
//     {
//         args.push_back(_identifierStr);
//     }

//     if (_currentToken != ')') {
//         logError("Expected ')' in prototype, found " + std::to_string(_currentToken));
//         return nullptr;
//     }

//     //success
//     getNextToken(); // eat ')'

//     return std::make_unique<PrototypeAST>(fnName, std::move(args));
// }

// /// definition ::= 'fun' prototype expression
// std::unique_ptr<FunctionAST> parseDefinition() {
//     getNextToken(); // eat fun

//     auto proto = parsePrototype();
//     if (!proto) {
//         return nullptr;
//     }

//     auto expr = parseExpression();
//     if (!expr) {
//         return nullptr;
//     }

//     return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
// }

// /// external ::= 'extern' prototype
// std::unique_ptr<PrototypeAST> parseExtern() {
//     getNextToken(); // eat extern

//     return parsePrototype();
// }

// /// toplevelexpr ::= expression
// std::unique_ptr<FunctionAST> parseTopLevelExpr() {
//     auto expr = parseExpression();
//     if (!expr) {
//         return nullptr;
//     }

//     auto proto = std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string>());
//     return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
// }

// //===----------------------------------------------------------------------===//
// // Top-Level parsing and JIT Driver
// //===----------------------------------------------------------------------===//

// static void HandleDefinition() {
//   if (auto FnAST = parseDefinition()) {
//     if (auto *FnIR = FnAST->codegen()) {
//       fprintf(stderr, "\nRead function definition:");
//       FnIR->print(llvm::errs());
//       fprintf(stderr, "\n");
//     }
//   } else {
//     // Skip token for error recovery.
//     getNextToken();
//   }
// }

// static void HandleExtern() {
//   if (auto ProtoAST = parseExtern()) {
//     if (auto *FnIR = ProtoAST->codegen()) {
//       fprintf(stderr, "Read extern: ");
//       FnIR->print(llvm::errs());
//       fprintf(stderr, "\n");
//     }
//   } else {
//     // Skip token for error recovery.
//     getNextToken();
//   }
// }

// static void HandleTopLevelExpression() {
//   // Evaluate a top-level expression into an anonymous function.
//   if (auto FnAST = parseTopLevelExpr()) {
//     if (auto *FnIR = FnAST->codegen()) {
//       fprintf(stderr, "Read top-level expression:");
//       FnIR->print(llvm::errs());
//       fprintf(stderr, "\n");
//     }
//   } else {
//     // Skip token for error recovery.
//     getNextToken();
//   }
// }

// // Main Loop
// /// top ::= definition | external | expression | ';'
// void mainLoop() {
//     while (1) {
//         fprintf(stderr, "ready > ");

//         switch (_currentToken) {
//         case TokenKind::eof:
//             return;
//         case ';': // ignore top-level semicolons.
//             getNextToken();
//             break;
//         case TokenKind::key_fun:
//             HandleDefinition();
//             break;
//         case TokenKind::key_extern:
//             HandleExtern();
//             break;
//         default:
//             HandleTopLevelExpression();
//             break;
//         }
//     }
// }

// int main() {
//     llvm::InitializeNativeTarget();
//     llvm::InitializeNativeTargetAsmPrinter();
//     llvm::InitializeNativeTargetAsmParser();

//     fprintf(stderr, "ready > ");
//     getNextToken();

//     initializeModuleAndPassManager();

//     // _module = std::make_unique<llvm::Module>("The JIT", _context);

//     mainLoop();

//     _module->print(llvm::errs(), nullptr);
// }

#include "KaleidoscopeJIT.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace llvm;
using namespace llvm::orc;

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5
};

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number

/// gettok - Return the next token from standard input.
static int gettok() {
  static int LastChar = ' ';

  // Skip any whitespace.
  while (isspace(LastChar))
    LastChar = getchar();

  if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    while (isalnum((LastChar = getchar())))
      IdentifierStr += LastChar;

    if (IdentifierStr == "def")
      return tok_def;
    if (IdentifierStr == "extern")
      return tok_extern;
    return tok_identifier;
  }

  if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(NumStr.c_str(), nullptr);
    return tok_number;
  }

  if (LastChar == '#') {
    // Comment until end of line.
    do
      LastChar = getchar();
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF)
      return gettok();
  }

  // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF)
    return tok_eof;

  // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  LastChar = getchar();
  return ThisChar;
}

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

namespace {

/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
  virtual ~ExprAST() = default;

  virtual Value *codegen() = 0;
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
  double Val;

public:
  NumberExprAST(double Val) : Val(Val) {}

  Value *codegen() override;
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
  std::string Name;

public:
  VariableExprAST(const std::string &Name) : Name(Name) {}

  Value *codegen() override;
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
  char Op;
  std::unique_ptr<ExprAST> LHS, RHS;

public:
  BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

  Value *codegen() override;
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<std::unique_ptr<ExprAST>> Args;

public:
  CallExprAST(const std::string &Callee,
              std::vector<std::unique_ptr<ExprAST>> Args)
      : Callee(Callee), Args(std::move(Args)) {}

  Value *codegen() override;
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAST(const std::string &Name, std::vector<std::string> Args)
      : Name(Name), Args(std::move(Args)) {}

  Function *codegen();
  const std::string &getName() const { return Name; }
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<ExprAST> Body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> Proto,
              std::unique_ptr<ExprAST> Body)
      : Proto(std::move(Proto)), Body(std::move(Body)) {}

  Function *codegen();
};

} // end anonymous namespace

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
static int CurTok;
static int getNextToken() { return CurTok = gettok(); }

/// BinopPrecedence - This holds the precedence for each binary operator that is
/// defined.
static std::map<char, int> BinopPrecedence;

/// GetTokPrecedence - Get the precedence of the pending binary operator token.
static int GetTokPrecedence() {
  if (!isascii(CurTok))
    return -1;

  // Make sure it's a declared binop.
  int TokPrec = BinopPrecedence[CurTok];
  if (TokPrec <= 0)
    return -1;
  return TokPrec;
}

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

static std::unique_ptr<ExprAST> ParseExpression();

/// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr() {
  auto Result = std::make_unique<NumberExprAST>(NumVal);
  getNextToken(); // consume the number
  return std::move(Result);
}

/// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr() {
  getNextToken(); // eat (.
  auto V = ParseExpression();
  if (!V)
    return nullptr;

  if (CurTok != ')')
    return LogError("expected ')'");
  getNextToken(); // eat ).
  return V;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr() {
  std::string IdName = IdentifierStr;

  getNextToken(); // eat identifier.

  if (CurTok != '(') // Simple variable ref.
    return std::make_unique<VariableExprAST>(IdName);

  // Call.
  getNextToken(); // eat (
  std::vector<std::unique_ptr<ExprAST>> Args;
  if (CurTok != ')') {
    while (true) {
      if (auto Arg = ParseExpression())
        Args.push_back(std::move(Arg));
      else
        return nullptr;

      if (CurTok == ')')
        break;

      if (CurTok != ',')
        return LogError("Expected ')' or ',' in argument list");
      getNextToken();
    }
  }

  // Eat the ')'.
  getNextToken();

  return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
static std::unique_ptr<ExprAST> ParsePrimary() {
  switch (CurTok) {
  default:
    return LogError("unknown token when expecting an expression");
  case tok_identifier:
    return ParseIdentifierExpr();
  case tok_number:
    return ParseNumberExpr();
  case '(':
    return ParseParenExpr();
  }
}

/// binoprhs
///   ::= ('+' primary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                              std::unique_ptr<ExprAST> LHS) {
  // If this is a binop, find its precedence.
  while (true) {
    int TokPrec = GetTokPrecedence();

    // If this is a binop that binds at least as tightly as the current binop,
    // consume it, otherwise we are done.
    if (TokPrec < ExprPrec)
      return LHS;

    // Okay, we know this is a binop.
    int BinOp = CurTok;
    getNextToken(); // eat binop

    // Parse the primary expression after the binary operator.
    auto RHS = ParsePrimary();
    if (!RHS)
      return nullptr;

    // If BinOp binds less tightly with RHS than the operator after RHS, let
    // the pending operator take RHS as its LHS.
    int NextPrec = GetTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS)
        return nullptr;
    }

    // Merge LHS/RHS.
    LHS =
        std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
  }
}

/// expression
///   ::= primary binoprhs
///
static std::unique_ptr<ExprAST> ParseExpression() {
  auto LHS = ParsePrimary();
  if (!LHS)
    return nullptr;

  return ParseBinOpRHS(0, std::move(LHS));
}

/// prototype
///   ::= id '(' id* ')'
static std::unique_ptr<PrototypeAST> ParsePrototype() {
  if (CurTok != tok_identifier)
    return LogErrorP("Expected function name in prototype");

  std::string FnName = IdentifierStr;
  getNextToken();

  if (CurTok != '(')
    return LogErrorP("Expected '(' in prototype");

  std::vector<std::string> ArgNames;
  while (getNextToken() == tok_identifier)
    ArgNames.push_back(IdentifierStr);
  if (CurTok != ')')
    return LogErrorP("Expected ')' in prototype");

  // success.
  getNextToken(); // eat ')'.

  return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

/// definition ::= 'def' prototype expression
static std::unique_ptr<FunctionAST> ParseDefinition() {
  getNextToken(); // eat def.
  auto Proto = ParsePrototype();
  if (!Proto)
    return nullptr;

  if (auto E = ParseExpression())
    return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
  return nullptr;
}

/// toplevelexpr ::= expression
static std::unique_ptr<FunctionAST> ParseTopLevelExpr() {
  if (auto E = ParseExpression()) {
    // Make an anonymous proto.
    auto Proto = std::make_unique<PrototypeAST>("__anon_expr",
                                                 std::vector<std::string>());
    return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
  }
  return nullptr;
}

/// external ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern() {
  getNextToken(); // eat extern.
  return ParsePrototype();
}

//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//

static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::map<std::string, Value *> NamedValues;
static std::unique_ptr<legacy::FunctionPassManager> TheFPM;
static std::unique_ptr<KaleidoscopeJIT> TheJIT;
static std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

Value *LogErrorV(const char *Str) {
  LogError(Str);
  return nullptr;
}

Function *getFunction(std::string Name) {
  // First, see if the function has already been added to the current module.
  if (auto *F = TheModule->getFunction(Name))
    return F;

  // If not, check whether we can codegen the declaration from some existing
  // prototype.
  auto FI = FunctionProtos.find(Name);
  if (FI != FunctionProtos.end())
    return FI->second->codegen();

  // If no existing prototype exists, return null.
  return nullptr;
}

Value *NumberExprAST::codegen() {
  return ConstantFP::get(TheContext, APFloat(Val));
}

Value *VariableExprAST::codegen() {
  // Look this variable up in the function.
  Value *V = NamedValues[Name];
  if (!V)
    return LogErrorV("Unknown variable name");
  return V;
}

Value *BinaryExprAST::codegen() {
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R)
    return nullptr;

  switch (Op) {
  case '+':
    return Builder.CreateFAdd(L, R, "addtmp");
  case '-':
    return Builder.CreateFSub(L, R, "subtmp");
  case '*':
    return Builder.CreateFMul(L, R, "multmp");
  case '<':
    L = Builder.CreateFCmpULT(L, R, "cmptmp");
    // Convert bool 0/1 to double 0.0 or 1.0
    return Builder.CreateUIToFP(L, Type::getDoubleTy(TheContext), "booltmp");
  default:
    return LogErrorV("invalid binary operator");
  }
}

Value *CallExprAST::codegen() {
  // Look up the name in the global module table.
  Function *CalleeF = getFunction(Callee);
  if (!CalleeF)
    return LogErrorV("Unknown function referenced");

  // If argument mismatch error.
  if (CalleeF->arg_size() != Args.size())
    return LogErrorV("Incorrect # arguments passed");

  std::vector<Value *> ArgsV;
  for (unsigned i = 0, e = Args.size(); i != e; ++i) {
    ArgsV.push_back(Args[i]->codegen());
    if (!ArgsV.back())
      return nullptr;
  }

  return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
}

Function *PrototypeAST::codegen() {
  // Make the function type:  double(double,double) etc.
  std::vector<Type *> Doubles(Args.size(), Type::getDoubleTy(TheContext));
  FunctionType *FT =
      FunctionType::get(Type::getDoubleTy(TheContext), Doubles, false);

  Function *F =
      Function::Create(FT, Function::ExternalLinkage, Name, TheModule.get());

  // Set names for all arguments.
  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName(Args[Idx++]);

  return F;
}

Function *FunctionAST::codegen() {
  // Transfer ownership of the prototype to the FunctionProtos map, but keep a
  // reference to it for use below.
  auto &P = *Proto;
  FunctionProtos[Proto->getName()] = std::move(Proto);
  Function *TheFunction = getFunction(P.getName());
  if (!TheFunction)
    return nullptr;

  // Create a new basic block to start insertion into.
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
  Builder.SetInsertPoint(BB);

  // Record the function arguments in the NamedValues map.
  NamedValues.clear();
  for (auto &Arg : TheFunction->args())
    NamedValues[std::string(Arg.getName())] = &Arg;

  if (Value *RetVal = Body->codegen()) {
    // Finish off the function.
    Builder.CreateRet(RetVal);

    // Validate the generated code, checking for consistency.
    verifyFunction(*TheFunction);

    // Run the optimizer on the function.
    TheFPM->run(*TheFunction);

    return TheFunction;
  }

  // Error reading body, remove function.
  TheFunction->eraseFromParent();
  return nullptr;
}

//===----------------------------------------------------------------------===//
// Top-Level parsing and JIT Driver
//===----------------------------------------------------------------------===//

static void InitializeModuleAndPassManager() {
  // Open a new module.
  TheModule = std::make_unique<Module>("my cool jit", TheContext);
  TheModule->setDataLayout(TheJIT->getTargetMachine().createDataLayout());

  // Create a new pass manager attached to it.
  TheFPM = std::make_unique<legacy::FunctionPassManager>(TheModule.get());

  // Do simple "peephole" optimizations and bit-twiddling optzns.
  TheFPM->add(createInstructionCombiningPass());
  // Reassociate expressions.
  TheFPM->add(createReassociatePass());
  // Eliminate Common SubExpressions.
  TheFPM->add(createGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  TheFPM->add(createCFGSimplificationPass());

  TheFPM->doInitialization();
}

static void HandleDefinition() {
  if (auto FnAST = ParseDefinition()) {
    if (auto *FnIR = FnAST->codegen()) {
      fprintf(stderr, "Read function definition:");
      FnIR->print(errs());
      fprintf(stderr, "\n");
      TheJIT->addModule(std::move(TheModule));
      InitializeModuleAndPassManager();
    }
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleExtern() {
  if (auto ProtoAST = ParseExtern()) {
    if (auto *FnIR = ProtoAST->codegen()) {
      fprintf(stderr, "Read extern: ");
      FnIR->print(errs());
      fprintf(stderr, "\n");
      FunctionProtos[ProtoAST->getName()] = std::move(ProtoAST);
    }
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (auto FnAST = ParseTopLevelExpr()) {
    if (FnAST->codegen()) {
      // JIT the module containing the anonymous expression, keeping a handle so
      // we can free it later.
      auto H = TheJIT->addModule(std::move(TheModule));
      InitializeModuleAndPassManager();

      // Search the JIT for the __anon_expr symbol.
      auto ExprSymbol = TheJIT->findSymbol("__anon_expr");
      assert(ExprSymbol && "Function not found");

      // Get the symbol's address and cast it to the right type (takes no
      // arguments, returns a double) so we can call it as a native function.
      double (*FP)() = (double (*)())(intptr_t)cantFail(ExprSymbol.getAddress());
      fprintf(stderr, "Evaluated to %f\n", FP());

      // Delete the anonymous expression module from the JIT.
      TheJIT->removeModule(H);
    }
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

/// top ::= definition | external | expression | ';'
static void MainLoop() {
  while (true) {
    fprintf(stderr, "ready> ");
    switch (CurTok) {
    case tok_eof:
      return;
    case ';': // ignore top-level semicolons.
      getNextToken();
      break;
    case tok_def:
      HandleDefinition();
      break;
    case tok_extern:
      HandleExtern();
      break;
    default:
      HandleTopLevelExpression();
      break;
    }
  }
}

//===----------------------------------------------------------------------===//
// "Library" functions that can be "extern'd" from user code.
//===----------------------------------------------------------------------===//

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

/// putchard - putchar that takes a double and returns 0.
extern "C" DLLEXPORT double putchard(double X) {
  fputc((char)X, stderr);
  return 0;
}

/// printd - printf that takes a double prints it as "%f\n", returning 0.
extern "C" DLLEXPORT double printd(double X) {
  fprintf(stderr, "%f\n", X);
  return 0;
}

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main() {
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();
  InitializeNativeTargetAsmParser();

  // Install standard binary operators.
  // 1 is lowest precedence.
  BinopPrecedence['<'] = 10;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40; // highest.

  // Prime the first token.
  fprintf(stderr, "ready> ");
  getNextToken();

  TheJIT = std::make_unique<KaleidoscopeJIT>();

  InitializeModuleAndPassManager();

  // Run the main "interpreter loop" now.
  MainLoop();

  return 0;
}
