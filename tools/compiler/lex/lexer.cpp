#include <iostream>
#include <cassert>
#include "zero/lex/lexer.h"

weasel::Lexer::Lexer(FileManager *fileManager)
{
    _currentBuffer = _startBuffer = fileManager->getStartBuffer();
    _endBuffer = _startBuffer + fileManager->getSize();
}

bool weasel::Lexer::compareBuffer(char *startBuffer, char *endBuffer, const char *compareBuffer)
{
    auto length = endBuffer - startBuffer;

    if (length != strlen(compareBuffer))
    {
        return false;
    }

    for (auto i = 0; i < length; i++)
    {
        if (startBuffer[i] != compareBuffer[i])
        {
            return false;
        }
    }

    return true;
}

bool weasel::Lexer::isIdentifier(char c, bool num)
{
    if (c == '_')
        return true;

    return num ? isalnum(c) : isalpha(c);
}

char *weasel::Lexer::getNextBuffer(size_t slide)
{
    _currentBuffer += slide; // get next buffer

    if (*_currentBuffer == '\n')
    {
        _location.row++;
        _location.col = 0; // new line didn't produce column value
    }
    else
    {
        _location.col++;
    }

    return _currentBuffer;
}

void weasel::Lexer::setCurrentBuffer(char *buffer)
{
    assert(buffer < _currentBuffer);
    _currentBuffer -= _currentBuffer - buffer;
}

bool weasel::Lexer::expect(weasel::TokenKind kind)
{
    auto *lastBuffer = _currentBuffer;
    auto ok = true;
    auto token = getToken();
    while (token->isKind(TokenKind::TokenSpaceNewline))
    {
        token = getToken();
    }

    if (!token->isKind(kind))
    {
        ok = false;
    }

    setCurrentBuffer(lastBuffer);
    return ok;
}

std::shared_ptr<weasel::Token> weasel::Lexer::createToken(weasel::TokenKind kind, char *startBuffer, char *endBuffer)
{
    return std::make_shared<Token>(kind, _location, startBuffer, endBuffer);
}

std::shared_ptr<weasel::Token> weasel::Lexer::getNextToken(bool skipSpace, bool eat)
{
    do
    {
        _currentToken = getToken();
    } while (_currentToken->isKind(TokenKind::TokenSpaceNewline) && skipSpace);

    return _currentToken;
}

std::shared_ptr<weasel::Token> weasel::Lexer::getToken()
{
    if (_currentBuffer == _endBuffer)
    {
        return createToken(TokenKind::TokenEOF, _currentBuffer, _endBuffer);
    }

    while (isspace(*_currentBuffer))
    {
        if (*_currentBuffer == '\n')
        {
            // New Line always come at the end of code
            // getNextChar will force new character from user input
            // And that's not what we want
            // Instead make lastChar empty char will not need new character
            // And next iteration will be ignored
            getNextBuffer();
            return createToken(TokenKind::TokenSpaceNewline, _currentBuffer - 1, _currentBuffer);
        }

        getNextBuffer();
    }

    // Check if identifier
    if (isIdentifier(*_currentBuffer))
    {
        auto *start = _currentBuffer;
        while (isIdentifier(*getNextBuffer(), true))
            ;

        /// Check Keyword ///
        auto ty = getKeyword(start, _currentBuffer);
        if (ty)
        {
            return ty;
        }

        // Check if data type
        ty = getType(start, _currentBuffer);
        if (ty)
        {
            return ty;
        }

        TokenKind kind = TokenKind::TokenIdentifier;
        // Check NIl Literal
        if (compareBuffer(start, _currentBuffer, "nil"))
        {
            kind = TokenKind::TokenLitNil;
        }
        else if (compareBuffer(start, _currentBuffer, "true") || compareBuffer(start, _currentBuffer, "false"))
        {
            // Check Boolean Literal
            kind = TokenKind::TokenLitBool;
        }

        // Identifier
        return createToken(kind, start, _currentBuffer);
    }

    // Check if Number
    if (isdigit(*_currentBuffer) || (*_currentBuffer == '.' && isdigit(*(_currentBuffer + 1))))
    {
        auto *start = _currentBuffer;
        while (isdigit(*getNextBuffer()) || *_currentBuffer == '.')
            ;

        // Number Literal
        return createToken(TokenKind::TokenLitNumber, start, _currentBuffer);
    }

    // String Literal
    if (*_currentBuffer == '"')
    {
        return getStringLiteral();
    }

    // Character Literal
    if (*_currentBuffer == '\'')
    {
        return getCharacterLiteral();
    }

    // Save Current Buffer
    auto *lastBuffer = _currentBuffer;
    getNextBuffer();

    // TODO: You need to save some comment to make a documentation
    // Single Line Comment
    if (*lastBuffer == '/' && *_currentBuffer == '/')
    {
        while (*getNextBuffer() != '\n' && isValidBuffer())
            ;
        return getToken();
    }

    // TODO: You need to save some comment to make a documentation
    // TODO: Should Error when EOF
    // Multiple Line Comment
    if (*lastBuffer == '/' && *_currentBuffer == '*')
    {
        lastBuffer = getNextBuffer();
        if (!isValidBuffer())
        {
            return getToken();
        }

        while (true)
        {
            getNextBuffer();
            if (!isValidBuffer())
            {
                return getToken();
            }

            if (*lastBuffer == '*' && *_currentBuffer == '/')
            {
                getNextBuffer(); // eat /
                return getToken();
            }

            lastBuffer = _currentBuffer;
        }
    }

    if (ispunct(*lastBuffer))
    {
        auto puncToken = getPunctuation();
        if (puncToken)
        {
            return puncToken;
        }
    }

    auto *start = _currentBuffer - 1;
    while (!isspace(*_currentBuffer))
    {
        getNextBuffer();
    }

    return createToken(TokenKind::TokenUndefined, start, _currentBuffer);
}
