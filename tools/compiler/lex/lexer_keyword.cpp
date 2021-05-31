#include "lex/lexer.h"

underrated::Token *underrated::Lexer::getKeyword(std::string key)
{
    if (key == "fun")
    {
        return createToken(TokenKind::TokenKeyFun);
    }
    if (key == "const")
    {
        return createToken(TokenKind::TokenKeyConst);
    }
    if (key == "export")
    {
        return createToken(TokenKind::TokenKeyExport);
    }
    if (key == "extern")
    {
        return createToken(TokenKind::TokenKeyExtern);
    }
    if (key == "return")
    {
        return createToken(TokenKind::TokenKeyReturn);
    }
    if (key == "let")
    {
        return createToken(TokenKind::TokenKeyLet);
    }
    if (key == "final")
    {
        return createToken(TokenKind::TokenKeyFinal);
    }
    if (key == "if")
    {
        return createToken(TokenKind::TokenKeyIf);
    }
    if (key == "else")
    {
        return createToken(TokenKind::TokenKeyElse);
    }
    if (key == "for")
    {
        return createToken(TokenKind::TokenKeyFor);
    }

    return nullptr;
}
