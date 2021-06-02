#include "ast/ast.h"
#include "analysis/context.h"

// Expression Codegen
llvm::Value *underrated::NumberLiteralExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *underrated::DeclarationExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *underrated::StatementExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *underrated::AssignmentExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *underrated::VariableExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *underrated::BinaryOperatorExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Value *underrated::ReturnExpression::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

llvm::Function *underrated::Function::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

// Expression
underrated::Type *underrated::BlockExpression::getVariable(std::string name)
{
    auto *ty = _variable[name];
    if (ty)
    {
        return ty;
    }

    if (getParent())
    {
        return getParent()->getVariable(name);
    }

    return nullptr;
}

// Function
void underrated::Function::setBody(StatementExpression *body)
{
    // body->setFunction(this);

    _body = body;
}

/// TODO: Debug
llvm::Value *underrated::DebugExpression::codegen(AnalysContext *context)
{
    llvm::errs() << *context->getModule();

    return nullptr;
}

/// Log Error
llvm::Value *underrated::logErrorV(std::string &msg)
{
    fprintf(stderr, "LogError LLVM : %s\n", msg);
    return nullptr;
}

underrated::Function *underrated::logErrorF(std::string &msg)
{
    fprintf(stderr, "LogError Func : %s\n", msg);
    return nullptr;
}
