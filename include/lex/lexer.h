#pragma once

#include "lex/token.h"

namespace underrated
{
    class Lexer
    {
    private:
        char _lastChar = ' ';
        Token *_currentToken;

    private:
        char getNextChar() { return _lastChar = getchar(); }

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

    public:
        Token *getNextToken(bool skipSpace = false);
        Token *getCurrentToken();
    };
} // namespace underrated
