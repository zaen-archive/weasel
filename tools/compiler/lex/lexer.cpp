#include <iostream>
#include "lex/lexer.h"

bool underrated::Lexer::isIdentifier(char c, bool num)
{
    if (c == '_')
        return true;

    return num ? isalnum(c) : isalpha(c);
}

char underrated::Lexer::getNextChar()
{
    _currentChar = getchar();
    if (_currentChar == '\n')
    {
        _location.row++;
        _location.col = 0;
    }
    else
    {
        _location.col++;
    }
    _location.position++;

    return _currentChar;
}

underrated::Token *underrated::Lexer::createToken(underrated::TokenKind kind)
{
    auto loc = _location;
    loc.length = 1;

    return new Token(kind, loc);
}

underrated::Token *underrated::Lexer::createToken(underrated::TokenKind kind, std::string val)
{
    auto loc = _location;
    loc.length = val.size();
    loc.col -= loc.length;

    return new Token(kind, loc, val);
}

underrated::Token *underrated::Lexer::getToken()
{
    while (isspace(_currentChar))
    {
        // New Line
        if (_currentChar == '\n')
        {
            // New Line always come at the end of code
            // getNextChar will force new character from user input
            // And that's not what we want
            // Instead make lastChar empty char will not need new character
            // And next iteration will be ignored
            _currentChar = ' ';
            return createToken(TokenKind::TokenSpaceNewline);
        }

        getNextChar();
    }

    // Check if identifier
    if (isIdentifier(_currentChar))
    {
        std::string identifier = std::string(1, _currentChar);
        while (isIdentifier(getNextChar(), true))
        {
            identifier += _currentChar;
        }

        /// Check Keyword ///
        auto *ty = getKeyword(identifier);
        if (ty)
        {
            return ty;
        }

        // Check if data type
        ty = getType(identifier);
        if (ty)
        {
            return ty;
        }

        // Check NIl Literal
        if (identifier == "nil")
        {
            return createToken(TokenKind::TokenLitNil);
        }

        // Check Boolean Literal
        if (identifier == "true" || identifier == "false")
        {
            return createToken(TokenKind::TokenLitBool);
        }

        // debug
        if (identifier == "debug")
        {
            return createToken(TokenKind::TokenDebug);
        }

        // Identifier
        return createToken(TokenKind::TokenIdentifier, identifier);
    }

    // Check if Number
    if (isdigit(_currentChar))
    {
        std::string numVal = std::string(1, _currentChar);
        while (isdigit(getNextChar()) || _currentChar == '.')
        {
            numVal += _currentChar;
        }

        // Number Literal
        return createToken(TokenKind::TokenLitNumber, numVal);
    }

    // String Literal
    if (_currentChar == '"')
    {
        getNextChar(); // eat '"'
        return getStringLiteral();
    }

    // Character Literal
    if (_currentChar == '\'')
    {
        getNextChar(); // eat '''

        return getCharacterLiteral();
    }

    // Save Last Char
    auto currentChar = _currentChar;
    getNextChar();

    // Punctuation
    if (ispunct(currentChar))
    {
        return getPunctuation(currentChar);
    }

    // TODO: You need to save some comment to make a documentation
    // Single Line Comment
    if (currentChar == '/' && _currentChar == '/')
    {
        while (getNextChar() != '\n')
            ;

        getNextChar(); // eat '\n'

        return getToken();
    }

    // TODO: You need to save some comment to make a documentation
    // Multiple Line Comment
    if (currentChar == '/' && _currentChar == '*')
    {
        currentChar = getNextChar();
        while (getNextChar() != '/' && currentChar != '*')
        {
            currentChar = _currentChar;
        }

        return getToken();
    }

    return createToken(TokenKind::TokenUndefined, std::string(1, currentChar));
}

underrated::Token *underrated::Lexer::getNextToken(bool skipSpace)
{
    do
    {
        _currentToken = getToken();
    } while (_currentToken->isKind(TokenKind::TokenSpaceNewline) && skipSpace);

    return _currentToken;
}

underrated::Token *underrated::Lexer::getCurrentToken()
{
    return _currentToken;
}
