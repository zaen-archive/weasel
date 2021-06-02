#include "lex/lexer.h"

underrated::Token *underrated::Lexer::getKeyword(std::string key)
{
    if (key == "fun")
    {
        return createToken(TokenKind::TokenKeyFun, key);
    }
    if (key == "const")
    {
        return createToken(TokenKind::TokenKeyConst, key);
    }
    if (key == "export")
    {
        return createToken(TokenKind::TokenKeyExport, key);
    }
    if (key == "extern")
    {
        return createToken(TokenKind::TokenKeyExtern, key);
    }
    if (key == "return")
    {
        return createToken(TokenKind::TokenKeyReturn, key);
    }
    if (key == "let")
    {
        return createToken(TokenKind::TokenKeyLet, key);
    }
    if (key == "final")
    {
        return createToken(TokenKind::TokenKeyFinal, key);
    }
    if (key == "if")
    {
        return createToken(TokenKind::TokenKeyIf, key);
    }
    if (key == "else")
    {
        return createToken(TokenKind::TokenKeyElse, key);
    }
    if (key == "for")
    {
        return createToken(TokenKind::TokenKeyFor, key);
    }

    return nullptr;
}
