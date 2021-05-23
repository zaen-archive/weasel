#include "ast/function.h"
#include "analysis/context.h"
#include "llvm/IR/Value.h"

llvm::Function *underrated::Func::codegen(AnalysContext *c)
{
    return c->codegen(this);
}

void underrated::Func::setBody(StmtExpression *body)
{
    if (_body)
    {
        delete _body;
    }

    _body = body;
}
