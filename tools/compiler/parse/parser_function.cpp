#include "parse/parser.h"

// define
// 'fun' identifier '(' args ')' funTy '{' stmt '}'
// declare
// 'fun' identifier '(' args ')' funTy
underrated::Func *underrated::Parser::parseFunction()
{
    getNextToken(); // eat 'fun'
    if (!getCurrentToken()->isKind(TokenKind::TokenIdentifier))
    {
        return logErrorF("Expected an identifier");
    }

    auto identifier = getCurrentToken()->getValue();

    getNextToken(); // eat 'identifier'
    if (!getCurrentToken()->isKind(TokenKind::TokenDelimOpenParen))
    {
        return logErrorF("Expected an open paren");
    }

    getNextToken(); // eat '('

    auto args = parseFunctionArguments();
    if (args.size() > 0)
    {
        if (!args.back())
        {
            return logErrorF("Expected an function argument");
        }
    }

    // Ignore new line
    while (getCurrentToken()->isKind(TokenKind::TokenSpaceNewline))
    {
        getNextToken();
    }

    auto *retTy = getCurrentToken();
    if (getCurrentToken()->isKind(TokenKind::TokenDelimOpenCurlyBracket))
    {
        retTy = new Token(TokenKind::TokenTyVoid);
    }
    else
    {
        getNextToken(); // eat data type
    }

    if (!retTy->isDataType())
    {
        return logErrorF("Expected return data type");
    }

    getNextToken(); // eat '{'

    auto *funcTy = new FuncType(retTy->toType(), args);
    return new Func(identifier, funcTy);
}

std::vector<underrated::FuncArg *> underrated::Parser::parseFunctionArguments()
{
    std::vector<underrated::FuncArg *> args;

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

underrated::FuncArg *underrated::Parser::parseFunctionArgument()
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

    return new FuncArg(identifier, token->toType());
}
