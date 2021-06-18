#include <iostream>
#include "weasel/lex/lexer.h"

std::shared_ptr<weasel::Token> weasel::Lexer::getKeyword(char *startBuffer, char *endBuffer)
{
    if (compareBuffer(startBuffer, endBuffer, "fun"))
    {
        return createToken(TokenKind::TokenKeyFun, startBuffer, endBuffer);
    }
    else if (compareBuffer(startBuffer, endBuffer, "const"))
    {
        return createToken(TokenKind::TokenKeyConst, startBuffer, endBuffer);
    }
    else if (compareBuffer(startBuffer, endBuffer, "extern"))
    {
        return createToken(TokenKind::TokenKeyExtern, startBuffer, endBuffer);
    }
    else if (compareBuffer(startBuffer, endBuffer, "return"))
    {
        return createToken(TokenKind::TokenKeyReturn, startBuffer, endBuffer);
    }
    else if (compareBuffer(startBuffer, endBuffer, "let"))
    {
        return createToken(TokenKind::TokenKeyLet, startBuffer, endBuffer);
    }
    else if (compareBuffer(startBuffer, endBuffer, "final"))
    {
        return createToken(TokenKind::TokenKeyFinal, startBuffer, endBuffer);
    }
    else if (compareBuffer(startBuffer, endBuffer, "if"))
    {
        return createToken(TokenKind::TokenKeyIf, startBuffer, endBuffer);
    }
    else if (compareBuffer(startBuffer, endBuffer, "else"))
    {
        return createToken(TokenKind::TokenKeyElse, startBuffer, endBuffer);
    }
    else if (compareBuffer(startBuffer, endBuffer, "for"))
    {
        return createToken(TokenKind::TokenKeyFor, startBuffer, endBuffer);
    }

    return nullptr;
}
