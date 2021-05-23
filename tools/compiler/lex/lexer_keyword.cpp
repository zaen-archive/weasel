#include "lex/lexer.h"

underrated::Token *underrated::Lexer::getKeyword(std::string key)
{
    if (key == "fun")
    {
        return new Token(TokenKind::TokenKeyFun);
    }
    if (key == "const")
    {
        return new Token(TokenKind::TokenKeyConst);
    }
    if (key == "export")
    {
        return new Token(TokenKind::TokenKeyExport);
    }
    if (key == "return")
    {
        return new Token(TokenKind::TokenKeyReturn);
    }
    if (key == "let")
    {
        return new Token(TokenKind::TokenKeyLet);
    }
    if (key == "final")
    {
        return new Token(TokenKind::TokenKeyFinal);
    }
    if (key == "if")
    {
        return new Token(TokenKind::TokenKeyIf);
    }
    if (key == "else")
    {
        return new Token(TokenKind::TokenKeyElse);
    }
    if (key == "for")
    {
        return new Token(TokenKind::TokenKeyFor);
    }

    return nullptr;
}
