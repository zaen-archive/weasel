#include "ast/type.h"
#include "analysis/context.h"

llvm::Type *underrated::IntType::codegen(AnalysContext *c)
{
    auto *builder = c->getBuilder();

    if (_valueWidth == 8)
    {
        return builder->getInt8Ty();
    }
    if (_valueWidth == 16)
    {
        return builder->getInt16Ty();
    }
    if (_valueWidth == 32)
    {
        return builder->getInt32Ty();
    }
    if (_valueWidth == 64)
    {
        return builder->getInt64Ty();
    }
    if (_valueWidth == 128)
    {
        return builder->getInt128Ty();
    }

    return builder->getInt32Ty();
}

llvm::Type *underrated::FloatType::codegen(AnalysContext *c)
{
    auto *builder = c->getBuilder();

    if (_valueWidth == 32)
    {
        return builder->getFloatTy();
    }
    if (_valueWidth == 64)
    {
        return builder->getDoubleTy();
    }
    if (_valueWidth == 128)
    {
        // TODO: Need more research
    }

    return builder->getFloatTy();
}

llvm::Type *underrated::VoidType::codegen(AnalysContext *c)
{
    return c->getBuilder()->getVoidTy();
}

llvm::Type *underrated::BoolType::codegen(AnalysContext *c)
{
    return c->getBuilder()->getInt1Ty();
}
