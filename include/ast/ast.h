#pragma once

#include <iostream>
#include "lex/token.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"

namespace underrated
{
    class ASTContext;

    class ExprAST
    {
    public:
        ExprAST() {}

    public:
        virtual void debug() { std::cout << "Base Class\n"; }
        virtual llvm::Value *codegen(ASTContext *context) = 0;
    };

    // TODO: Debug
    class DebugExprAST : public ExprAST
    {
    public:
        llvm::Value *codegen(ASTContext *context);

    public:
        void debug() override
        {
            std::cout << "DebugAST\n";
        }
    };

    class NumberExprAST : public ExprAST
    {
    private:
        int _value;

    public:
        NumberExprAST(int value) : _value(value) {}
        int getValue() const { return _value; }
        llvm::Value *codegen(ASTContext *context);

    public:
        void debug() override
        {
            std::cout << "NumberExprAST : " << _value << "\n";
        }
    };

    class VariableExprAST : public ExprAST
    {
    private:
        std::string _identifier;
        ExprAST *_value;

    public:
        VariableExprAST(std::string identifier, ExprAST *value) : _identifier(identifier), _value(value) {}
        std::string getIdentifier() const { return _identifier; }
        ExprAST *getValue() const { return _value; }
        llvm::Value *codegen(ASTContext *context);

    public:
        void debug() override
        {
            std::cout << "VariableExprAST : " << _identifier << "\n";
            _value->debug();
        }
    };

    class BinopExprAST : public ExprAST
    {
    private:
        ExprAST *_lhs;
        ExprAST *_rhs;

    public:
        BinopExprAST(ExprAST *lhs, ExprAST *rhs) : _lhs(lhs), _rhs(rhs) {}
        llvm::Value *codegen(ASTContext *context);

    public:
        void debug() override
        {
            std::cout << "BinopExprAST\n";
        }
    };

    ExprAST *logError(const char *msg);
    ExprAST *logErrorV(const char *msg);
} // namespace underrated
