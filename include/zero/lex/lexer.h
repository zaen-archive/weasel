#pragma once

#include <fstream>
#include "zero/lex/token.h"

namespace zero
{
    class Lexer
    {
    private:
        char _currentChar = ' ';
        Token *_currentToken;
        Token *_lastToken;
        SourceLocation _location;
        std::ifstream *_stream;

    private:
        bool isIdentifier(char c, bool num = false);
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
        Lexer(std::ifstream *stream) : _stream(stream) {}

        Token *getNextToken(bool skipSpace = false);
        Token *getCurrentToken() const { return _currentToken; }
        Token *getLastToken() const { return _lastToken; }

        void setLastToken(Token *token) { _lastToken = token; }
    };

} // namespace zero
