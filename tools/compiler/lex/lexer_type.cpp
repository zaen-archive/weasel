#include "lex/lexer.h"

underrated::Token *underrated::Lexer::getType(std::string type)
{
    if (type == "rune")
    {
        return new Token(TokenKind::TokenTyRune, type);
    }
    if (type == "byte")
    {
        return new Token(TokenKind::TokenTyByte, type);
    }
    if (type == "sbyte")
    {
        return new Token(TokenKind::TokenTySbyte, type);
    }
    if (type == "short")
    {
        return new Token(TokenKind::TokenTyShort, type);
    }
    if (type == "ushort")
    {
        return new Token(TokenKind::TokenTyUshort, type);
    }
    if (type == "int")
    {
        return new Token(TokenKind::TokenTyInt, type);
    }
    if (type == "uint")
    {
        return new Token(TokenKind::TokenTyUint, type);
    }
    if (type == "long")
    {
        return new Token(TokenKind::TokenTyLong, type);
    }
    if (type == "ulong")
    {
        return new Token(TokenKind::TokenTyUlong, type);
    }
    if (type == "int128")
    {
        return new Token(TokenKind::TokenTyInt128, type);
    }
    if (type == "bool")
    {
        return new Token(TokenKind::TokenTyBool, type);
    }
    if (type == "float")
    {
        return new Token(TokenKind::TokenTyFloat, type);
    }
    if (type == "double")
    {
        return new Token(TokenKind::TokenTyDouble, type);
    }
    if (type == "decimal")
    {
        return new Token(TokenKind::TokenTyDecimal, type);
    }

    return nullptr;
}
