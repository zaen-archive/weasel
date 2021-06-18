#include "weasel/ast/ast.h"
#include "weasel/analysis/context.h"

// Expression Codegen
llvm::Value *weasel::NumberLiteralExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::DeclarationExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::StatementExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::VariableExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::ArrayExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::StringLiteralExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::BinaryOperatorExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::CallExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::ReturnExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::ArrayLiteralExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::NilLiteralExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Function *weasel::Function::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

/// Log Error
llvm::Value *weasel::logErrorV(std::string msg)
{
    fprintf(stderr, "LogError LLVM : %s\n", msg.c_str());
    return nullptr;
}

llvm::Function *weasel::logErrorF(std::string msg)
{
    fprintf(stderr, "LogError Func : %s\n", msg.c_str());
    return nullptr;
}
