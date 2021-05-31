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
    // Compare Type
    enum class CompareType
    {
        Equal,
        Casting,
        Different,
    };

    // Analysis Context
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

        llvm::Value castIntegerType(llvm::Value *lhs, llvm::Value *rhs);
        llvm::Value *castIntegerType(llvm::Value *value, llvm::Type *castTy);
        CompareType compareType(llvm::Type *lhsType, llvm::Type *rhsType);
        std::string getDefaultLabel();

    public:
        llvm::Value *codegen(VariableExpression *expr);
        llvm::Value *codegen(AssignmentExpression *expr);
        llvm::Value *codegen(NumberLiteralExpression *expr);
        llvm::Value *codegen(StatementExpression *expr);
        llvm::Value *codegen(ReturnExpression *expr);
        llvm::Value *codegen(BinaryOperatorExpression *expr); // TODO: Need to implement modulo operator
        llvm::Function *codegen(Function *func);
    };
} // namespace underrated
