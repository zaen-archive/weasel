#include "analysis/context.h"
#include "lex/token.h"
#include "ast/ast.h"

llvm::Type *underrated::Token::toType(AnalysContext *c)
{
    auto *builder = c->getBuilder();

    // Integer
    if (isKind(TokenKind::TokenTyByte) || isKind(TokenKind::TokenTySbyte))
    {
        // return new Type(builder->getInt8Ty(), qualifier, isKind(TokenKind::TokenTySbyte));
        return builder->getInt8Ty();
    }
    if (isKind(TokenKind::TokenTyShort) || isKind(TokenKind::TokenTyUshort))
    {
        // return new Type(builder->getInt16Ty(), qualifier, isKind(TokenKind::TokenTyShort));
        return builder->getInt16Ty();
    }
    if (isKind(TokenKind::TokenTyInt) || isKind(TokenKind::TokenTyUint))
    {
        // return new Type(builder->getInt32Ty(), qualifier, isKind(TokenKind::TokenTyInt));
        return builder->getInt32Ty();
    }
    if (isKind(TokenKind::TokenTyLong) || isKind(TokenKind::TokenTyUlong))
    {
        // return new Type(builder->getInt64Ty(), qualifier, isKind(TokenKind::TokenTyLong));
        return builder->getInt64Ty();
    }
    if (isKind(TokenKind::TokenTyInt128) || isKind(TokenKind::TokenTyUnt128))
    {
        // return new Type(builder->getInt128Ty(), qualifier, isKind(TokenKind::TokenTyInt128));
        return builder->getInt128Ty();
    }

    // Float
    if (isKind(TokenKind::TokenTyFloat))
    {
        // return new Type(builder->getFloatTy());
        return builder->getFloatTy();
    }
    if (isKind(TokenKind::TokenTyDouble))
    {
        // return new Type(builder->getDoubleTy());
        return builder->getDoubleTy();
    }

    // TODO: Need more research
    // if (isKind(TokenKind::TokenTyDecimal))
    // {
    //     return new Type(builder->getDecimalTy());
    // }

    // Bool
    if (isKind(TokenKind::TokenTyBool))
    {
        // return new Type(builder->getInt1Ty());
        return builder->getInt1Ty();
    }

    // Void
    // return new Type(builder->getVoidTy());
    return builder->getVoidTy();
}

underrated::Qualifier underrated::Token::getQualifier()
{
    switch (getTokenKind())
    {
    case TokenKind::TokenKeyConst:
        return Qualifier::QualConst;
    case TokenKind::TokenKeyFinal:
        return Qualifier::QualRestrict;
    default:
        return Qualifier::QualVolatile;
    }
}

underrated::Precedence underrated::Token::getPrecedence()
{
    Precedence val;
    val.associative = Associative::LeftToRight;
    val.order = 15;

    if (_kind == TokenKind::TokenPuncMinus || _kind == TokenKind::TokenPuncPlus)
    {
        val.order = 6;
    }
    else if (_kind == TokenKind::TokenPuncStar || _kind == TokenKind::TokenPuncSlash || _kind == TokenKind::TokenPuncPercent)
    {
        val.order = 5;
    }

    return val;
}
