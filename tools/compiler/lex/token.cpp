#include "analysis/context.h"
#include "lex/token.h"
#include "ast/type.h"

underrated::Type *underrated::Token::toType(AnalysContext *c, Qualifier qualifier)
{
    auto *builder = c->getBuilder();

    // Integer
    if (isKind(TokenKind::TokenTyByte) || isKind(TokenKind::TokenTySbyte))
    {
        return new Type(builder->getInt8Ty(), qualifier, isKind(TokenKind::TokenTySbyte));
    }
    if (isKind(TokenKind::TokenTyShort) || isKind(TokenKind::TokenTyUshort))
    {
        return new Type(builder->getInt16Ty(), qualifier, isKind(TokenKind::TokenTyShort));
    }
    if (isKind(TokenKind::TokenTyInt) || isKind(TokenKind::TokenTyUint))
    {
        return new Type(builder->getInt32Ty(), qualifier, isKind(TokenKind::TokenTyInt));
    }
    if (isKind(TokenKind::TokenTyLong) || isKind(TokenKind::TokenTyUlong))
    {
        return new Type(builder->getInt64Ty(), qualifier, isKind(TokenKind::TokenTyLong));
    }
    if (isKind(TokenKind::TokenTyInt128) || isKind(TokenKind::TokenTyUnt128))
    {
        // No Unsigned long
        return new Type(builder->getInt128Ty(), qualifier, isKind(TokenKind::TokenTyInt128));
    }

    // Float
    if (isKind(TokenKind::TokenTyFloat))
    {
        return new Type(builder->getFloatTy());
    }
    if (isKind(TokenKind::TokenTyDouble))
    {
        return new Type(builder->getDoubleTy());
    }

    // TODO: Need more research
    // if (isKind(TokenKind::TokenTyDecimal))
    // {
    //     return new Type(builder->getDecimalTy());
    // }

    // Bool
    if (isKind(TokenKind::TokenTyBool))
    {
        return new Type(builder->getInt1Ty());
    }

    // Void
    return new Type(builder->getVoidTy());
}
