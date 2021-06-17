#pragma once

#include <fstream>
#include <memory>
#include "zero/lex/token.h"
#include "zero/basic/filemanager.h"

namespace weasel
{
    class Lexer
    {
    private:
        char *_startBuffer;
        char *_endBuffer;
        char *_currentBuffer;

        std::shared_ptr<Token> _currentToken;
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
        std::shared_ptr<Token> getToken();
        std::shared_ptr<Token> createToken(TokenKind kind, char *startBuffer, char *endBuffer);
        std::shared_ptr<Token> getType(char *startBuffer, char *endBuffer);
        std::shared_ptr<Token> getKeyword(char *startBuffer, char *endBuffer);
        std::shared_ptr<Token> getPunctuation();
        std::shared_ptr<Token> getStringLiteral();
        std::shared_ptr<Token> getCharacterLiteral();

    public:
        Lexer(FileManager *fileManager);

        std::shared_ptr<Token> getNextToken(bool skipSpace = false, bool eat = false);
        std::shared_ptr<Token> getCurrentToken() const { return _currentToken; }
        bool expect(TokenKind kind);
    };

} // namespace weasel
