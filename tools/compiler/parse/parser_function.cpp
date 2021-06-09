#include "llvm/IR/Type.h"
#include "llvm/IR/Instructions.h"
#include "zero/parse/parser.h"
#include "zero/analysis/context.h"
#include "zero/symbol/symbol.h"

// define
// 'fun' identifier '(' args ')' funTy '{' stmt '}'
std::shared_ptr<zero::Function> zero::Parser::parseFunction()
{
    auto func = parseDeclareFunction();
    if (!func)
    {
        return nullptr;
    }
    func->setIsDefine(true);

    // Ignore new line
    if (getCurrentToken()->isKind(TokenKind::TokenSpaceNewline))
    {
        getNextToken(true);
    }

    if (!getCurrentToken()->isKind(TokenKind::TokenDelimOpenCurlyBracket))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected {");
    }

    // Set Symbol for parameters and enter a scope
    {
        SymbolTable::getInstance().enterScope();
        for (auto arg : func->getArgs())
        {
            auto argName = arg->getArgumentName();
            auto ty = arg->getArgumentType();
            auto attr = std::make_shared<Attribute>(argName, AttributeScope::ScopeParam, AttributeKind::SymbolParameter, ty);

            SymbolTable::getInstance().insert(argName, attr);
        }
    }

    auto body = parseFunctionBody();
    if (!body)
    {
        return ErrorTable::addError(getCurrentToken(), "Expected valid body statement!.");
    }

    // Exit parameter scope
    {
        SymbolTable::getInstance().exitScope();
    }

    func->setBody(body);
    return func;
}

// extern 'fun' identifier '(' args ')' funTy
std::shared_ptr<zero::Function> zero::Parser::parseDeclareFunction()
{
    if (!getCurrentToken()->isKind(TokenKind::TokenKeyFun))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected fun keyword");
    }

    // get next and eat 'fun'
    if (!getNextToken()->isKind(TokenKind::TokenIdentifier))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected an identifier");
    }

    auto identifier = getCurrentToken()->getValue();
    getNextToken(); // eat 'identifier'
    if (!getCurrentToken()->isKind(TokenKind::TokenDelimOpenParen))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected (");
    }

    getNextToken(); // eat '('
    auto args = parseFunctionArguments();
    if (args.size() > 0)
    {
        if (!args.back())
        {
            return ErrorTable::addError(getCurrentToken(), "Expected function argument");
        }
    }

    llvm::Type *returnType;
    if (getCurrentToken()->isDataType())
    {
        returnType = getCurrentToken()->toType(getContext());

        getNextToken(); // eat 'data type'
    }
    else
    {
        returnType = llvm::Type::getVoidTy(*getContext()->getContext());
    }

    auto funcTy = std::make_shared<FunctionType>(returnType, args);
    auto func = std::make_shared<Function>(identifier, funcTy);

    // Check Symbol Table
    if (SymbolTable::getInstance().get(identifier))
    {
        return ErrorTable::addError(getCurrentToken(), "Function already declared");
    }

    // Create Symbol for the function
    {
        SymbolTable::getInstance().insert(identifier, std::make_shared<Attribute>(identifier, AttributeScope::ScopeGlobal, AttributeKind::SymbolFunction, returnType));
    }

    return func;
}

std::vector<std::shared_ptr<zero::FunctionArgument>> zero::Parser::parseFunctionArguments()
{
    std::vector<std::shared_ptr<zero::FunctionArgument>> args;

    while (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseParen))
    {
        args.push_back(parseFunctionArgument());

        if (args[args.size() - 1] == nullptr)
        {
            // immeadiate return to inform there is wrong with arguments
            return args;
        }

        if (!getCurrentToken()->isKind(TokenKind::TokenPuncComma))
        {
            break;
        }

        getNextToken(); // eat ','
    }

    if (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseParen))
    {
        ErrorTable::addError(getCurrentToken(), "Expected ) in function argument");
        if (!getCurrentToken()->isKind(TokenKind::TokenDelimOpenSquareBracket))
        {

            getNextTokenUntil(TokenKind::TokenDelimCloseParen);
        }
    }
    else
    {
        getNextToken(); // eat ')'
    }

    return args;
}

std::shared_ptr<zero::FunctionArgument> zero::Parser::parseFunctionArgument()
{
    if (!getCurrentToken()->isKind(TokenKind::TokenIdentifier))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected identifier in function argument");
    }

    auto identifier = getCurrentToken()->getValue();
    getNextToken();

    auto *type = parseDataType();
    if (!type)
    {
        return ErrorTable::addError(getCurrentToken(), "Expected type in function argument");
    }

    return std::make_shared<FunctionArgument>(identifier, type);
}
