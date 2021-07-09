#include "weasel/ir/context.h"
#include "weasel/lex/token.h"
#include "weasel/ast/ast.h"

llvm::Type *weasel::Token::toType(llvm::LLVMContext &c, bool pointerTy) const
{
    llvm::Type *type = nullptr;
    if (isKind(TokenKind::TokenTyByte) || isKind(TokenKind::TokenTySbyte))
    {
        type = llvm::Type::getInt8Ty(c);
    }
    else if (isKind(TokenKind::TokenTyShort) || isKind(TokenKind::TokenTyUshort))
    {
        type = llvm::Type::getInt16Ty(c);
    }
    else if (isKind(TokenKind::TokenTyInt) || isKind(TokenKind::TokenTyUint))
    {
        type = llvm::Type::getInt32Ty(c);
    }
    else if (isKind(TokenKind::TokenTyLong) || isKind(TokenKind::TokenTyUlong))
    {
        type = llvm::Type::getInt64Ty(c);
    }
    else if (isKind(TokenKind::TokenTyInt128) || isKind(TokenKind::TokenTyUnt128))
    {
        type = llvm::Type::getInt128Ty(c);
    }
    else if (isKind(TokenKind::TokenTyFloat))
    {
        type = llvm::Type::getFloatTy(c);
    }
    else if (isKind(TokenKind::TokenTyDouble))
    {
        type = llvm::Type::getDoubleTy(c);
    }
    else if (isKind(TokenKind::TokenTyBool))
    {
        type = llvm::Type::getInt1Ty(c);
    }
    else if (isKind(TokenKind::TokenTyVoid))
    {
        type = llvm::Type::getVoidTy(c);
    }

    if (pointerTy)
    {
        return llvm::PointerType::get(type, 0);
    }

    return type;
}

weasel::Qualifier weasel::Token::getQualifier() const
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
weasel::Precedence weasel::Token::getPrecedence()
{
    Precedence val{};
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
        val.order = __defaultPrecOrder;
        break;
    }

    return val;
}
