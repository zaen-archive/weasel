#include "lex/lexer.h"

underrated::Token *underrated::Lexer::getToken()
{
    while (isspace(_lastChar))
    {
        // Line Feed same as Semicolon
        if (_lastChar == '\n')
        {
            _lastChar = ' ';
            return new Token(TokenKind::TokenSpaceNewline);
        }

        getNextChar();
    }

    // Check if identifier
    if (isalpha(_lastChar))
    {
        std::string identifier = std::string(1, _lastChar);
        while (isalnum(getNextChar()))
        {
            identifier += _lastChar;
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
            return new Token(TokenKind::TokenLitNil);
        }

        // Check Boolean Literal
        if (identifier == "true" || identifier == "false")
        {
            return new Token(TokenKind::TokenLitBool);
        }

        // debug
        if (identifier == "debug")
        {
            return new Token(TokenKind::TokenDebug);
        }

        // Identifier
        return new Token(TokenKind::TokenIdentifier, identifier);
    }

    // Check if Number
    if (isdigit(_lastChar))
    {
        std::string numVal = std::string(1, _lastChar);
        while (isdigit(getNextChar()) || _lastChar == '.')
        {
            numVal += _lastChar;
        }

        // Number Literal
        return new Token(TokenKind::TokenLitNumber, numVal);
    }

    // String Literal
    if (_lastChar == '"')
    {
        getNextChar(); // eat '"'
        return getStringLiteral();
    }

    // Character Literal
    if (_lastChar == '\'')
    {
        getNextChar(); // eat '''

        return getCharacterLiteral();
    }

    // Save Last Char
    auto currentChar = _lastChar;
    getNextChar();

    // TODO: You need to save some comment to make a documentation
    // Single Line Comment
    if (currentChar == '/' && _lastChar == '/')
    {
        while (getNextChar() != '\n')
            ;

        getNextChar(); // eat '\n'

        return getToken();
    }

    // TODO: You need to save some comment to make a documentation
    // Multiple Line Comment
    if (currentChar == '/' && _lastChar == '*')
    {
        currentChar = getNextChar();
        while (getNextChar() != '/' && currentChar != '*')
        {
            currentChar = _lastChar;
        }

        return getToken();
    }

    // Punctuation
    if (ispunct(currentChar))
    {
        return getPunctuation(currentChar);
    }

    return new Token(TokenKind::TokenUndefined, std::string(1, currentChar));
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
