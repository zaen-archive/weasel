#include "parse/parser.h"
#include "analysis/context.h"

// define
// 'fun' identifier '(' args ')' funTy '{' stmt '}'
underrated::Function *underrated::Parser::parseFunction()
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
        return logErrorF("Expected '{'");
    }

    auto *body = parseFunctionBody();
    if (!body)
    {
        return logErrorF("Expected valid body statement!.");
    }

    func->setBody(body);

    return func;
}

// extern 'fun' identifier '(' args ')' funTy
underrated::Function *underrated::Parser::parseDeclareFunction()
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
        retTy = new Token(TokenKind::TokenTyVoid, retTy->getLocation());
    }

    auto *funcTy = new FunctionType(retTy->toType(getContext()), args);
    auto *func = new Function(identifier, funcTy);

    return func;
}

std::vector<underrated::FunctionArgument *> underrated::Parser::parseFunctionArguments()
{
    std::vector<underrated::FunctionArgument *> args;

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

underrated::FunctionArgument *underrated::Parser::parseFunctionArgument()
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
