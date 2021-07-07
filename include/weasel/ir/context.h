#pragma once

#include <map>
#include <string>
#include <sstream>
#include "llvm/IR/Value.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/MDBuilder.h"
#include "weasel/ast/ast.h"

namespace weasel
{
    // Compare Type
    enum class CompareType
    {
        Equal,
        Casting,
        Different,
    };

    // Analysis Context
    class Context
    {
    private:
        llvm::Module *_module;
        llvm::MDBuilder *_mdBuilder;
        llvm::LLVMContext *_context;
        llvm::IRBuilder<> *_builder;
        Function *_currentFunction = nullptr;

        bool _isHostCL = false;
        bool _isParallel = false;
        unsigned long _counter = 0;

    private:
        llvm::MDNode *getTBAA(llvm::Type *type) const;
        llvm::MDNode *getTBAARoot() const;
        llvm::MDNode *getTBAAChar() const;
        llvm::MDNode *getTBAAShort() const;
        llvm::MDNode *getTBAAInt() const;
        llvm::MDNode *getTBAALong() const;
        llvm::MDNode *getTBAAPointer() const;

        void parallelRun() const;
        void parallelInit() const;
        void parallelInitkernel(const std::string &kernelName);
        void parallelInitArgument(llvm::Value *arr, int size) const;
        void parallelDestroy() const;

    public:
        explicit Context(llvm::LLVMContext *context, const std::string &moduleName, bool isParallel = false);

        llvm::LLVMContext *getContext() const { return _context; }
        llvm::Module *getModule() const { return _module; }
        llvm::IRBuilder<> *getBuilder() const { return _builder; }
        llvm::MDBuilder *getMDBuilder() const { return _mdBuilder; }

        void setHostCL(bool val) { _isHostCL = val; }
        bool isParallel() const { return _isParallel; }

        llvm::Value *castIntegerType(llvm::Value *lhs, llvm::Value *rhs) const;
        llvm::Value *castIntegerType(llvm::Value *value, llvm::Type *castTy);
        CompareType compareType(llvm::Type *lhsType, llvm::Type *rhsType);
        std::string getDefaultLabel();

    public:
        llvm::Value *codegen(VariableExpression *expr) const;
        llvm::Value *codegen(NumberLiteralExpression *expr) const;
        llvm::Value *codegen(StatementExpression *expr);
        llvm::Value *codegen(CallExpression *expr);
        llvm::Value *codegen(ReturnExpression *expr);
        llvm::Value *codegen(DeclarationExpression *expr);
        llvm::Value *codegen(StringLiteralExpression *expr) const;
        llvm::Value *codegen() const;
        llvm::Value *codegen(ArrayLiteralExpression *expr);
        llvm::Value *codegen(ArrayExpression *expr);
        // TODO: Need to implement modulo operator
        llvm::Value *codegen(BinaryOperatorExpression *expr);
        llvm::Function *codegen(Function *func);
    };

} // namespace weasel
