#include "weasel/lex/lexer.h"

std::shared_ptr<weasel::Token> weasel::Lexer::getType(char *startBuffer, char *endBuffer)
{
    TokenKind kind;
    if (compareBuffer(startBuffer, endBuffer, "rune"))
    {
        kind = TokenKind::TokenTyRune;
    }
    else if (compareBuffer(startBuffer, endBuffer, "byte"))
    {
        kind = TokenKind::TokenTyByte;
    }
    else if (compareBuffer(startBuffer, endBuffer, "sbyte"))
    {
        kind = TokenKind::TokenTySbyte;
    }
    else if (compareBuffer(startBuffer, endBuffer, "short"))
    {
        kind = TokenKind::TokenTyShort;
    }
    else if (compareBuffer(startBuffer, endBuffer, "ushort"))
    {
        kind = TokenKind::TokenTyUshort;
    }
    else if (compareBuffer(startBuffer, endBuffer, "int"))
    {
        kind = TokenKind::TokenTyInt;
    }
    else if (compareBuffer(startBuffer, endBuffer, "uint"))
    {
        kind = TokenKind::TokenTyUint;
    }
    else if (compareBuffer(startBuffer, endBuffer, "long"))
    {
        kind = TokenKind::TokenTyLong;
    }
    else if (compareBuffer(startBuffer, endBuffer, "ulong"))
    {
        kind = TokenKind::TokenTyUlong;
    }
    else if (compareBuffer(startBuffer, endBuffer, "int128"))
    {
        kind = TokenKind::TokenTyInt128;
    }
    else if (compareBuffer(startBuffer, endBuffer, "bool"))
    {
        kind = TokenKind::TokenTyBool;
    }
    else if (compareBuffer(startBuffer, endBuffer, "float"))
    {
        kind = TokenKind::TokenTyFloat;
    }
    else if (compareBuffer(startBuffer, endBuffer, "double"))
    {
        kind = TokenKind::TokenTyDouble;
    }
    else if (compareBuffer(startBuffer, endBuffer, "decimal"))
    {
        kind = TokenKind::TokenTyDecimal;
    }
    else if (compareBuffer(startBuffer, endBuffer, "void"))
    {
        kind = TokenKind::TokenTyVoid;
    }
    else
    {
        return nullptr;
    }

    return createToken(kind, startBuffer, endBuffer);
}
