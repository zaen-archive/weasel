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
        Token *getToken();

    public:
        Token *getNextToken();
        Token *getCurrentToken();
        int getTokenPrecedence();
    };
} // namespace underrated
