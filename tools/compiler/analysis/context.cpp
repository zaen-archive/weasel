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

llvm::Value *underrated::AnalysContext::codegen(LiteralExpression *expr)
{
    return llvm::ConstantInt::get(getBuilder()->getInt32Ty(), expr->getValue());
}

llvm::Function *underrated::AnalysContext::codegen(Func *func)
{
    if (getModule()->getFunction(func->getIdentifier()))
    {
        return nullptr;
    }

    auto funcArgs = func->getArgs();
    auto *retTy = func->getType()->getLLVMType();
    auto args = std::vector<llvm::Type *>();
    if (funcArgs.size() > 0)
    {
        for (auto &item : funcArgs)
        {
            args.push_back(item->getLLVMType());
        }
    }

    auto *funcTy = llvm::FunctionType::get(retTy, args, false);
    auto *f = llvm::Function::Create(funcTy, llvm::Function::ExternalWeakLinkage, func->getIdentifier(), *getModule());
    auto idx = 0;
    for (auto &item : f->args())
    {
        item.setName(funcArgs[idx++]->getArgName());
    }

    // Create Block
    auto *block = llvm::BasicBlock::Create(*getContext(), "entry", f);

    return f;
}
