#include "zero/parse/parser.h"
#include "zero/analysis/context.h"
#include "zero/symbol/symbol.h"

// define
// 'fun' identifier '(' args ')' funTy '{' stmt '}'
zero::Function *zero::Parser::parseFunction()
{
    auto *func = parseDeclareFunction();
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
        return logErrorF(std::string("Expected '{'"));
    }

    // Set Symbol for parameters and enter a scope
    {
        SymbolTable::getInstance().enterScope();
        for (auto *arg : func->getArgs())
        {
            auto argName = arg->getArgumentName();
            auto *ty = arg->getArgumentType();
            auto *attr = new Attribute(argName, AttributeScope::ScopeParam, AttributeKind::SymbolParameter, ty);

            SymbolTable::getInstance().insert(argName, attr);
        }
    }

    auto *body = parseFunctionBody();
    if (!body)
    {
        return logErrorF(std::string("Expected valid body statement!."));
    }

    // Exit parameter scope
    {
        SymbolTable::getInstance().exitScope();
    }

    func->setBody(body);
    return func;
}

// extern 'fun' identifier '(' args ')' funTy
zero::Function *zero::Parser::parseDeclareFunction()
{
    if (!getCurrentToken()->isKind(TokenKind::TokenKeyFun))
    {
        return logErrorF(std::string("Expected fun keyword"));
    }

    getNextToken(); // eat 'fun'
    if (!getCurrentToken()->isKind(TokenKind::TokenIdentifier))
    {
        return logErrorF(std::string("Expected an identifier"));
    }

    auto identifier = getCurrentToken()->getValue();

    getNextToken(); // eat 'identifier'
    if (!getCurrentToken()->isKind(TokenKind::TokenDelimOpenParen))
    {
        return logErrorF(std::string("Expected an open paren"));
    }

    getNextToken(); // eat '('
    auto args = parseFunctionArguments();
    if (args.size() > 0)
    {
        if (!args.back())
        {
            return logErrorF(std::string("Expected an function argument"));
        }
    }

    auto *retTy = getCurrentToken();
    if (retTy->isDataType())
    {
        getNextToken(); // eat 'data type'
    }
    else
    {
        retTy = new Token(TokenKind::TokenTyVoid, retTy->getLocation());
    }

    auto *returnType = retTy->toType(getContext());
    auto *funcTy = new FunctionType(returnType, args);
    auto *func = new Function(identifier, funcTy);

    // Check Symbol Table
    if (SymbolTable::getInstance().get(identifier))
    {
        return logErrorF(std::string("Function already declared"));
    }

    // Create Symbol for the function
    SymbolTable::getInstance().insert(identifier, new Attribute(identifier, AttributeScope::ScopeGlobal, AttributeKind::SymbolFunction, returnType));

    return func;
}

std::vector<zero::FunctionArgument *> zero::Parser::parseFunctionArguments()
{
    std::vector<zero::FunctionArgument *> args;

    while (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseParen))
    {
        args.push_back(parseFunctionArgument());

        if (args[args.size() - 1] == nullptr)
        {
            // immeadiate return to inform there is wrong with arguments
            return args;
        }
    }

    getNextToken(); // eat ')'

    return args;
}

zero::FunctionArgument *zero::Parser::parseFunctionArgument()
{
    auto *token = getCurrentToken();
    if (!token->isKind(TokenKind::TokenIdentifier))
    {
        return nullptr;
    }

    auto identifier = token->getValue();
    token = getNextToken();
    if (!token->isDataType())
    {
        return nullptr;
    }

    if (getNextToken()->isKind(TokenKind::TokenPuncComma))
    {
        getNextToken(); // eat ','
    }

    return new FunctionArgument(identifier, token->toType(getContext()));
}
