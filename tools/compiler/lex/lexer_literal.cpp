#include "zero/lex/lexer.h"

std::shared_ptr<zero::Token> zero::Lexer::getStringLiteral()
{
    auto *start = getNextBuffer(); // eat double quote (")
    while (*getNextBuffer() != '"')
        ;

    auto *endString = _currentBuffer;

    getNextBuffer(); // eat double quote (")
    return createToken(TokenKind::TokenLitString, start, endString);
}

std::shared_ptr<zero::Token> zero::Lexer::getCharacterLiteral()
{
    auto *start = getNextBuffer(); // eat single quote (')
    if (*start == '\'')
    {
        return nullptr;
    }

    if (*getNextBuffer() != '\'')
    {
        return nullptr;
    }

    getNextBuffer(); // eat single quote (')
    return createToken(TokenKind::TokenLitChar, start, start + 1);
}
