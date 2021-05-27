#pragma once

#include <map>
#include <string>
#include "llvm/IR/Value.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "ast/ast.h"

namespace underrated
{
    class AnalysContext
    {
    private:
        llvm::Module *_module;
        llvm::LLVMContext *_context;
        llvm::IRBuilder<> *_builder;

        unsigned long long _counter = 0;

    public:
        AnalysContext();
        AnalysContext(std::string moduleName);

        llvm::LLVMContext *getContext() const { return _context; }
        llvm::Module *getModule() const { return _module; }
        llvm::IRBuilder<> *getBuilder() const { return _builder; }

        std::string getDefaultLabel();

    public:
        llvm::Value *codegen(VariableExpression *expr);
        llvm::Value *codegen(AssignmentExpression *expr);
        llvm::Value *codegen(NumberLiteralExpression *expr);
        llvm::Value *codegen(StatementExpression *expr);
        llvm::Value *codegen(ReturnExpression *expr);
        llvm::Function *codegen(Function *func);
    };
} // namespace underrated
