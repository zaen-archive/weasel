#pragma once

#include <vector>
#include <string>
#include "llvm/IR/Function.h"
#include "ast/type.h"
#include "ast/expr.h"

namespace underrated
{
    class AnalysContext;

    class Func
    {
    private:
        std::string _identifier;
        FuncType *_funcTy;
        StmtExpression *_body;

    public:
        Func(std::string identifier, FuncType *funcTy) : _identifier(identifier), _funcTy(funcTy) {}

        std::string getIdentifier() const { return _identifier; }
        Type *getType() const { return _funcTy->getType(); }
        std::vector<FuncArg *> getArgs() const { return _funcTy->getArgs(); }

        void setBody(StmtExpression *body);

    public:
        llvm::Function *codegen(AnalysContext *c);
    };
} // namespace underrated
