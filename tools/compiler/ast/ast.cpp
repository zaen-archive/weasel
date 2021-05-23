// TODO: Debug : Verify Function
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Value.h"
#include "ast/ast.h"
#include "analysis/context.h"

/// Log Error
underrated::Expression *underrated::logError(const char *msg)
{
    fprintf(stderr, "LogError: %s\n", msg);
    return nullptr;
}

underrated::Expression *underrated::logErrorV(const char *msg)
{
    fprintf(stderr, "LogError LLVM : %s\n", msg);
    return nullptr;
}

underrated::Func *underrated::logErrorF(const char *msg)
{
    fprintf(stderr, "LogError Func : %s\n", msg);
    return nullptr;
}

// /// Codegen
// llvm::Value *underrated::LiteralExpr::codegen(AnalysContext *context)
// {
//     return context->codegen(this);
// }

// llvm::Value *underrated::VariableExpr::codegen(AnalysContext *context)
// {
//     return context->codegen(this);
// }

// llvm::Value *underrated::BinopExpr::codegen(AnalysContext *context)
// {
//     return context->codegen(this);
// }

// llvm::Function *underrated::Func::codegen(AnalysContext *c)
// {
//     return c->codegen(this);
// }

// /// TODO: Debug
// llvm::Value *underrated::DebugExpression::codegen(AnalysContext *context)
// {
//     llvm::errs() << *context->getModule();

//     return nullptr;
// }
