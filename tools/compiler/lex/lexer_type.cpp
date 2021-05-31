#include "lex/lexer.h"

underrated::Token *underrated::Lexer::getType(std::string type)
{
    if (type == "rune")
    {
        return createToken(TokenKind::TokenTyRune, type);
    }
    if (type == "byte")
    {
        return createToken(TokenKind::TokenTyByte, type);
    }
    if (type == "sbyte")
    {
        return createToken(TokenKind::TokenTySbyte, type);
    }
    if (type == "short")
    {
        return createToken(TokenKind::TokenTyShort, type);
    }
    if (type == "ushort")
    {
        return createToken(TokenKind::TokenTyUshort, type);
    }
    if (type == "int")
    {
        return createToken(TokenKind::TokenTyInt, type);
    }
    if (type == "uint")
    {
        return createToken(TokenKind::TokenTyUint, type);
    }
    if (type == "long")
    {
        return createToken(TokenKind::TokenTyLong, type);
    }
    if (type == "ulong")
    {
        return createToken(TokenKind::TokenTyUlong, type);
    }
    if (type == "int128")
    {
        return createToken(TokenKind::TokenTyInt128, type);
    }
    if (type == "bool")
    {
        return createToken(TokenKind::TokenTyBool, type);
    }
    if (type == "float")
    {
        return createToken(TokenKind::TokenTyFloat, type);
    }
    if (type == "double")
    {
        return createToken(TokenKind::TokenTyDouble, type);
    }
    if (type == "decimal")
    {
        return createToken(TokenKind::TokenTyDecimal, type);
    }

    return nullptr;
}
