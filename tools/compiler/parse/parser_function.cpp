#include "parse/parser.h"

// define
// 'fun' identifier '(' args ')' funTy '{' stmt '}'
// declare
// 'fun' identifier '(' args ')' funTy
underrated::Func *underrated::Parser::parseFunction()
{
    auto *func = parseDeclareFunction();

    // Ignore new line
    while (getCurrentToken()->isKind(TokenKind::TokenSpaceNewline))
    {
        getNextToken();
    }

    std::cout << enumToInt(getCurrentToken()->getTokenKind()) << ":" << getCurrentToken()->getValue() << "\n";

    if (!getCurrentToken()->isKind(TokenKind::TokenDelimOpenCurlyBracket))
    {
        return logErrorF("Expected '{'");
    }

    getNextToken(); // eat '{'

    // auto *body = parseBody();

    return func;
}

underrated::Func *underrated::Parser::parseDeclareFunction()
{
    if (!getCurrentToken()->isKind(TokenKind::TokenKeyFun))
    {
        return logErrorF("Expected fun keyword");
    }

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

    auto *retTy = getCurrentToken();
    if (retTy->isDataType())
    {
        getNextToken(); // eat 'data type'
    }
    else
    {
        retTy = new Token(TokenKind::TokenTyVoid);
    }

    auto *funcTy = new FuncType(retTy->toType(getContext()), args);
    auto *func = new Func(identifier, funcTy);

    return func;
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

    return new FuncArg(identifier, token->toType(getContext()));
}
