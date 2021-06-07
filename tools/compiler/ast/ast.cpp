#include "zero/ast/ast.h"
#include "zero/analysis/context.h"

// Expression Codegen
llvm::Value *zero::NumberLiteralExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *zero::DeclarationExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *zero::StatementExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *zero::AssignmentExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *zero::VariableExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *zero::BinaryOperatorExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *zero::ReturnExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Function *zero::Function::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

// Function
void zero::Function::setBody(StatementExpression *body)
{
    // body->setFunction(this);

    _body = body;
}

/// Log Error
llvm::Value *zero::logErrorV(std::string &msg)
{
    fprintf(stderr, "LogError LLVM : %s\n", msg.c_str());
    return nullptr;
}

zero::Function *zero::logErrorF(std::string &msg)
{
    fprintf(stderr, "LogError Func : %s\n", msg.c_str());
    return nullptr;
}
