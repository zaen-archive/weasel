#include "lex/lexer.h"
#include <iostream>

underrated::Token *underrated::Lexer::getToken()
{
    while (isspace(_lastChar))
    {
        // Line Feed same as Semicolon
        if (_lastChar == '\n')
        {
            _lastChar = ' ';
            return new Token(TokenKind::TokenSemicolon);
        }

        _lastChar = getchar();
    }

    // Check if identifier
    if (isalpha(_lastChar))
    {
        std::string identifier = std::string(1, _lastChar);
        while (isalnum(_lastChar = getchar()))
        {
            identifier += _lastChar;
        }

        /// Check Keyword ///
        // fun
        if (identifier == "fun")
        {
            return new Token(TokenKind::TokenKeyFun, identifier);
        }

        // extern
        if (identifier == "extern")
        {
            return new Token(TokenKind::TokenKeyExtern, identifier);
        }

        // let
        if (identifier == "let")
        {
            return new Token(TokenKind::TokenKeyLet, identifier);
        }

        // int
        if (identifier == "int")
        {
            return new Token(TokenKind::TokenKeyInt, identifier);
        }

        // debug
        if (identifier == "debug")
        {
            return new Token(TokenKind::TokenDebug, identifier);
        }

        // Identifier
        return new Token(TokenKind::TokenIdentifier, identifier);
    }

    // Check if Number
    if (isdigit(_lastChar))
    {
        std::string numVal = std::string(1, _lastChar);
        while (isdigit(_lastChar = getchar()) || _lastChar == '.')
        {
            numVal += _lastChar;
        }

        // Check valid number

        return new Token(TokenKind::TokenNumber, numVal);
    }

    auto currentChar = _lastChar;
    auto currentStr = std::string(1, currentChar);
    _lastChar = getchar();

    // Semicolon
    if (currentChar == ';')
    {
        return new Token(TokenKind::TokenSemicolon);
    }

    // Operator
    if (currentChar == '+')
    {
        return new Token(TokenKind::TokenOpPlus, currentStr);
    }
    if (currentChar == '-')
    {
        return new Token(TokenKind::TokenOpMinus, currentStr);
    }
    if (currentChar == '*')
    {
        return new Token(TokenKind::TokenOpMul, currentStr);
    }
    if (currentChar == '/')
    {
        return new Token(TokenKind::TokenOpDivide, currentStr);
    }
    if (currentChar == '=')
    {
        return new Token(TokenKind::TokenOpAssign, currentStr);
    }

    // Parentise
    if (currentChar == '(')
    {
        return new Token(TokenKind::TokenOpenParen, currentStr);
    }
    if (currentChar == ')')
    {
        return new Token(TokenKind::TokenCloseParen, currentStr);
    }

    // Comma
    if (currentChar == ',')
    {
        return new Token(TokenKind::TokenComma, currentStr);
    }

    return new Token(TokenKind::TokenUndefined, currentStr);
}

underrated::Token *underrated::Lexer::getNextToken()
{
    return _currentToken = getToken();
}

underrated::Token *underrated::Lexer::getCurrentToken()
{
    return _currentToken;
}

int underrated::Lexer::getTokenPrecedence()
{
    auto val = _currentToken->getTokenKind();
    switch (val)
    {
    case TokenKind::TokenOpPlus:
    case TokenKind::TokenOpMinus:
        return 10;
    case TokenKind::TokenOpMul:
    case TokenKind::TokenOpDivide:
    case TokenKind::TokenOpMod:
        return 20;
    default:
        return -1;
    }
}
