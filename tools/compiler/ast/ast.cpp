// TODO: Debug : Verify Function
#include "llvm/IR/Verifier.h"
#include "ast/ast.h"
#include "ast/ast_context.h"

underrated::ExprAST *underrated::logError(const char *msg)
{
    fprintf(stderr, "LogError: %s\n", msg);
    return nullptr;
}

underrated::ExprAST *underrated::logErrorV(const char *msg)
{
    fprintf(stderr, "LogError LLVM : %s\n", msg);
    return nullptr;
}

llvm::Value *underrated::NumberExprAST::codegen(ASTContext *context)
{
    return context->codegen(this);
}

llvm::Value *underrated::VariableExprAST::codegen(ASTContext *context)
{
    return context->codegen(this);
}

llvm::Value *underrated::BinopExprAST::codegen(ASTContext *context)
{
    return context->codegen(this);
}

// TODO: Debug
llvm::Value *underrated::DebugExprAST::codegen(ASTContext *context)
{
    llvm::verifyFunction(*context->getDefaultFunction());
    llvm::errs() << *context->getModule();

    return nullptr;
}
