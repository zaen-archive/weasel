#include "lex/token.h"

underrated::Type *underrated::Token::toType()
{
    // Integer
    if (isKind(TokenKind::TokenTyByte) || isKind(TokenKind::TokenTySbyte))
    {
        return IntType::getInt8Ty(isKind(TokenKind::TokenTySbyte));
    }

    if (isKind(TokenKind::TokenTyShort) || isKind(TokenKind::TokenTyUshort))
    {
        return IntType::getInt16Ty(isKind(TokenKind::TokenTyShort));
    }

    if (isKind(TokenKind::TokenTyInt) || isKind(TokenKind::TokenTyUint))
    {
        return IntType::getInt32Ty(isKind(TokenKind::TokenTyInt));
    }

    if (isKind(TokenKind::TokenTyLong) || isKind(TokenKind::TokenTyUlong))
    {
        return IntType::getInt64Ty(isKind(TokenKind::TokenTyLong));
    }

    if (isKind(TokenKind::TokenTyInt128) || isKind(TokenKind::TokenTyUnt128))
    {
        return IntType::getInt128Ty(isKind(TokenKind::TokenTyInt128));
    }

    // Float
    if (isKind(TokenKind::TokenTyFloat))
    {
        return FloatType::getFloatTy();
    }

    if (isKind(TokenKind::TokenTyDouble))
    {
        return FloatType::getDoubleTy();
    }

    if (isKind(TokenKind::TokenTyDecimal))
    {
        return FloatType::getDecimalTy();
    }

    // Bool
    if (isKind(TokenKind::TokenTyBool))
    {
        return new BoolType();
    }

    return new VoidType();
}
