#include <iostream>
#include "analysis/context.h"

underrated::AnalysContext::AnalysContext()
{
    _context = new llvm::LLVMContext();
    _module = new llvm::Module("underrated", *getContext());
    _builder = new llvm::IRBuilder<>(*getContext());
}

underrated::AnalysContext::AnalysContext(std::string moduleName)
{
    _context = new llvm::LLVMContext();
    _module = new llvm::Module(moduleName, *getContext());
    _builder = new llvm::IRBuilder<>(*getContext());
}

std::string underrated::AnalysContext::getDefaultLabel()
{
    return std::to_string(_counter++);
}

llvm::Function *underrated::AnalysContext::codegen(Function *func)
{
    if (getModule()->getFunction(func->getIdentifier()))
    {
        return nullptr;
    }

    auto funcArgs = func->getArgs();
    auto *retTy = func->getFunctionType()->getReturnType();
    auto args = std::vector<llvm::Type *>();
    if (funcArgs.size() > 0)
    {
        for (auto &item : funcArgs)
        {
            args.push_back(item->getArgumentType());
        }
    }

    auto *funcTy = llvm::FunctionType::get(retTy, args, false);
    auto *f = llvm::Function::Create(funcTy, llvm::Function::ExternalWeakLinkage, func->getIdentifier(), *getModule());
    auto idx = 0;
    for (auto &item : f->args())
    {
        item.setName(funcArgs[idx++]->getArgumentName());
    }

    if (func->getIsDefine())
    {
        auto *entry = llvm::BasicBlock::Create(*getContext(), "entry", f);
        getBuilder()->SetInsertPoint(entry);

        // Create Block
        if (func->getBody())
        {
            func->getBody()->codegen(this);
        }
    }

    return f;
}

llvm::Value *underrated::AnalysContext::codegen(StatementExpression *expr)
{
    for (auto &item : expr->getBody())
    {
        item->codegen(this);
    }

    return nullptr;
}

llvm::Value *underrated::AnalysContext::codegen(NumberLiteralExpression *expr)
{
    return getBuilder()->getInt64(expr->getValue());
}

llvm::Value *underrated::AnalysContext::codegen(AssignmentExpression *expr)
{
    auto *rhs = expr->getRHS()->codegen(this);
    auto *lhs = expr->getLHS()->codegen(this);

    return getBuilder()->CreateStore(rhs, lhs);
}

llvm::Value *underrated::AnalysContext::codegen(ReturnExpression *expr)
{
    if (expr->getValue())
    {
        return getBuilder()->CreateRet(expr->getValue()->codegen(this));
    }
    else
    {
        return getBuilder()->CreateRetVoid();
    }
}

// TODO: Handle just definition variable
llvm::Value *underrated::AnalysContext::codegen(VariableExpression *expr)
{
    return getBuilder()->CreateAlloca(getBuilder()->getInt64Ty(), 0, expr->getIdentifier());
}
