#include "weasel/analysis/context.h"

// Compare Type Helpter
weasel::CompareType weasel::AnalysContext::compareType(llvm::Type *lhsType, llvm::Type *rhsType)
{
    if (lhsType->getTypeID() != rhsType->getTypeID())
    {
        return CompareType::Different;
    }

    if (lhsType->isIntegerTy())
    {
        if (lhsType->getIntegerBitWidth() != rhsType->getIntegerBitWidth())
        {
            return CompareType::Casting;
        }
    }

    return CompareType::Equal;
}

// Cast Integer Type Helper
llvm::Value *weasel::AnalysContext::castIntegerType(llvm::Value *value, llvm::Type *castTy)
{
    if (value->getType()->getIntegerBitWidth() < castTy->getIntegerBitWidth())
    {
        return getBuilder()->CreateCast(llvm::Instruction::CastOps::SExt, value, castTy);
    }
    else
    {
        return getBuilder()->CreateCast(llvm::Instruction::CastOps::Trunc, value, castTy);
    }
}

// Cast Integer Type Helper
llvm::Value *weasel::AnalysContext::castIntegerType(llvm::Value *lhs, llvm::Value *rhs)
{
    if (lhs->getType()->getIntegerBitWidth() > rhs->getType()->getIntegerBitWidth())
    {
        return getBuilder()->CreateCast(llvm::Instruction::CastOps::SExt, rhs, lhs->getType());
    }
    else
    {
        return getBuilder()->CreateCast(llvm::Instruction::CastOps::SExt, lhs, rhs->getType());
    }
}
