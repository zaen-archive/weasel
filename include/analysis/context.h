#pragma once

#include <map>
#include <string>
#include "llvm/IR/Value.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "ast/function.h"
#include "ast/expr.h"

namespace underrated
{
    class AnalysContext
    {
    private:
        llvm::Module *_module;
        llvm::LLVMContext *_context;
        std::map<std::string, llvm::AllocaInst *> _namedValues;
        llvm::IRBuilder<> *_builder;

    public:
        AnalysContext();
        AnalysContext(std::string moduleName);

        llvm::LLVMContext *getContext() const { return _context; }
        llvm::Module *getModule() const { return _module; }
        llvm::IRBuilder<> *getBuilder() const { return _builder; }

    public:
        llvm::Value *codegen(LiteralExpr *expr);
        llvm::Function *codegen(Func *func);
    };
} // namespace underrated
