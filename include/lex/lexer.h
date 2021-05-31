#pragma once

#include "lex/token.h"

namespace underrated
{
    class Lexer
    {
    private:
        char _currentChar = ' ';
        Token *_currentToken;
        SourceLocation _location;

    private:
        bool isIdentifier(char c, bool num = false);

    private:
        char getNextChar();

        Token *getToken();
        Token *getType(std::string type);
        Token *getKeyword(std::string key);
        Token *getPunctuation(char leftChar);

        // Literal
        Token *getStringLiteral();
        Token *getCharacterLiteral();
        // Direct implemented happend because the lexer quite simple
        // Token *getNumberLiteral();   -> Implemented at getToken directly
        // Token *getBooleanLiteral();  -> Implemented at getToken directly
        // Token *getNilLiteral();      -> Implemented at getToken directly

        // Create New Token
        Token *createToken(TokenKind kind, std::string val);
        Token *createToken(TokenKind kind);

    public:
        Token *getNextToken(bool skipSpace = false);
        Token *getCurrentToken();
    };

} // namespace underrated
