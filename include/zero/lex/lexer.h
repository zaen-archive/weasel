#pragma once

#include <fstream>
#include "zero/lex/token.h"
#include "zero/basic/filemanager.h"

namespace zero
{
    class Lexer
    {
    private:
        char *_startBuffer;
        char *_endBuffer;
        char *_currentBuffer;

        Token *_currentToken;
        SourceLocation _location;

    private:
        // Get and Next Buffer
        char *getNextBuffer(size_t slide = 1);
        bool compareBuffer(char *startBuffer, char *endBuffer, const char *compareBuffer);
        bool isIdentifier(char c, bool num = false);
        void setCurrentBuffer(char *buffer);
        bool isValidBuffer() const { return _endBuffer - _currentBuffer > 0; }

        // TODO: Handle special character fir parseCharacter or string
        // Token section
        Token *getToken();
        Token *createToken(TokenKind kind, char *startBuffer, char *endBuffer);
        Token *getType(char *startBuffer, char *endBuffer);
        Token *getKeyword(char *startBuffer, char *endBuffer);
        Token *getPunctuation();
        Token *getStringLiteral();
        Token *getCharacterLiteral();
        // Token *getNumberLiteral();   -> Implemented at getToken directly
        // Token *getBooleanLiteral();  -> Implemented at getToken directly
        // Token *getNilLiteral();      -> Implemented at getToken directly

    public:
        Lexer(FileManager *fileManager);

        Token *getNextToken(bool skipSpace = false, bool eat = false);
        Token *getCurrentToken() const { return _currentToken; }
        bool expect(TokenKind kind);
    };

} // namespace zero
