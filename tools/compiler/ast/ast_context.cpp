#include "ast/ast_context.h"
#include "ast/ast.h"

underrated::ASTContext::ASTContext()
{
    _context = new llvm::LLVMContext();
    _module = new llvm::Module("underrated JIT", getContext());
    _builder = new llvm::IRBuilder<>(getContext());

    // TODO: Default Function, Testing Purpose
    _defFun = llvm::Function::Create(llvm::FunctionType::get(getBuilder()->getVoidTy(), false), llvm::Function::ExternalLinkage, "defFun", *getModule());
    _defBlock = llvm::BasicBlock::Create(getContext(), "entry", _defFun);
    _builder->SetInsertPoint(_defBlock);
}

llvm::Value *underrated::ASTContext::codegen(NumberExprAST *expr)
{
    return llvm::ConstantInt::get(getBuilder()->getInt32Ty(), expr->getValue());
}

llvm::Value *underrated::ASTContext::codegen(VariableExprAST *expr)
{
    if (_namedValues[expr->getIdentifier()])
    {
        return nullptr;
    }

    // Store Variable in Memory
    auto *alloc = _builder->CreateAlloca(_builder->getInt32Ty(), 0, expr->getIdentifier().c_str());
    _builder->CreateStore(expr->getValue()->codegen(this), alloc);
    return _namedValues[expr->getIdentifier()] = alloc;
}

llvm::Value *underrated::ASTContext::codegen(BinopExprAST *expr)
{
    return llvm::ConstantFP::get(getContext(), llvm::APFloat(10.0));
}
