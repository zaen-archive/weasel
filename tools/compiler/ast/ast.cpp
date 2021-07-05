#include "weasel/ast/ast.h"
#include "weasel/ir/context.h"

// Expression Codegen
llvm::Value *weasel::NumberLiteralExpression::codegen(Context *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::DeclarationExpression::codegen(Context *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::StatementExpression::codegen(Context *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::VariableExpression::codegen(Context *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::ArrayExpression::codegen(Context *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::StringLiteralExpression::codegen(Context *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::BinaryOperatorExpression::codegen(Context *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::CallExpression::codegen(Context *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::ReturnExpression::codegen(Context *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::ArrayLiteralExpression::codegen(Context *c)
{
    return c->codegen(this);
}

llvm::Value *weasel::NilLiteralExpression::codegen(Context *c)
{
    return c->codegen();
}

llvm::Function *weasel::Function::codegen(Context *c)
{
    return c->codegen(this);
}
