#pragma once

#include <map>
#include <string>
#include "llvm/IR/Value.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

namespace underrated
{
    class NumberExprAST;
    class VariableExprAST;
    class BinopExprAST;

    class ASTContext
    {
    private:
        llvm::Module *_module;
        llvm::LLVMContext *_context;
        std::map<std::string, llvm::AllocaInst *> _namedValues;
        llvm::IRBuilder<> *_builder;

        // Default Function Mode
        // TODO: Testing Function
    private:
        llvm::Function *_defFun;
        llvm::BasicBlock *_defBlock;

    public:
        ASTContext();
        llvm::LLVMContext &getContext() { return *_context; }
        llvm::Module *getModule() const { return _module; }
        llvm::IRBuilder<> *getBuilder() const { return _builder; }
        llvm::Function *getDefaultFunction() const { return _defFun; }

    public:
        llvm::Value *codegen(NumberExprAST *expr);
        llvm::Value *codegen(VariableExprAST *expr);
        llvm::Value *codegen(BinopExprAST *expr);
    };
} // namespace underrated
