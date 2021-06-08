#include "zero/analysis/context.h"
#include "zero/lex/token.h"
#include "zero/ast/ast.h"

llvm::Type *zero::Token::toType(AnalysContext *c)
{
    auto *builder = c->getBuilder();

    // Integer
    if (isKind(TokenKind::TokenTyByte) || isKind(TokenKind::TokenTySbyte))
    {
        return builder->getInt8Ty();
    }
    if (isKind(TokenKind::TokenTyShort) || isKind(TokenKind::TokenTyUshort))
    {
        return builder->getInt16Ty();
    }
    if (isKind(TokenKind::TokenTyInt) || isKind(TokenKind::TokenTyUint))
    {
        return builder->getInt32Ty();
    }
    if (isKind(TokenKind::TokenTyLong) || isKind(TokenKind::TokenTyUlong))
    {
        return builder->getInt64Ty();
    }
    if (isKind(TokenKind::TokenTyInt128) || isKind(TokenKind::TokenTyUnt128))
    {
        return builder->getInt128Ty();
    }

    // Float
    if (isKind(TokenKind::TokenTyFloat))
    {
        return builder->getFloatTy();
    }
    if (isKind(TokenKind::TokenTyDouble))
    {
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
        return builder->getInt1Ty();
    }

    // Void
    return builder->getVoidTy();
}

zero::Qualifier zero::Token::getQualifier()
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

// TODO: Need to add associativity
zero::Precedence zero::Token::getPrecedence()
{
    Precedence val;
    val.associative = Associative::LeftToRight;

    switch (_kind)
    {
    case TokenKind::TokenPuncDot:
    case TokenKind::TokenPuncDotThree:
    case TokenKind::TokenDelimOpenSquareBracket:
    case TokenKind::TokenDelimOpenParen:
        val.order = 2;
        break;
    case TokenKind::TokenOperatorStar:
    case TokenKind::TokenOperatorSlash:
    case TokenKind::TokenOperatorPercent:
        val.order = 5;
        break;
    case TokenKind::TokenOperatorMinus:
    case TokenKind::TokenOperatorPlus:
        val.order = 6;
        break;
    case TokenKind::TokenOperatorShiftLeft:
    case TokenKind::TokenOperatorShiftRight:
        val.order = 7;
        break;
    case TokenKind::TokenOperatorLessThan:
    case TokenKind::TokenOperatorLessEqual:
    case TokenKind::TokenOperatorGreaterThen:
    case TokenKind::TokenOperatorGreaterEqual:
        val.order = 9;
        break;
    case TokenKind::TokenOperatorEqualEqual:
    case TokenKind::TokenOperatorNotEqual:
        val.order = 10;
        break;
    case TokenKind::TokenOperatorAnd:
        val.order = 11;
        break;
    case TokenKind::TokenOperatorCaret:
        val.order = 12;
        break;
    case TokenKind::TokenOperatorNot:
        val.order = 13;
        break;
    case TokenKind::TokenOperatorAndAnd:
        val.order = 14;
        break;
    case TokenKind::TokenOperatorOror:
        val.order = 15;
        break;
    case TokenKind::TokenOperatorEqual:
    case TokenKind::TokenOperatorPlusEqual:
    case TokenKind::TokenOperatorMinusEqual:
    case TokenKind::TokenOperatorStarEqual:
    case TokenKind::TokenOperatorSlashEqual:
    case TokenKind::TokenOperatorPercentEqual:
    case TokenKind::TokenOperatorShiftLeftEqual:
    case TokenKind::TokenOperatorShiftRightEqual:
    case TokenKind::TokenOperatorAndEqual:
    case TokenKind::TokenOperatorCaretEqual:
        // case TokenKind::TokenOperatorNotEqual:
        val.associative = Associative::RightToLeft;
        val.order = 16;
        break;
    default:
        val.order = defPrecOrder;
        break;
    }

    return val;
}
