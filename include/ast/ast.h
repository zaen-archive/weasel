#pragma once

#include <iostream>
#include "lex/token.h"

namespace underrated
{
    class ExprAST
    {
    public:
        ExprAST() {}

    public:
        virtual void debug() { std::cout << "Base Class\n"; }
    };

    class NumberExprAST : public ExprAST
    {
    private:
        double _value;

    public:
        NumberExprAST(double value) : _value(value) {}

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

    public:
        void debug() override
        {
            std::cout << "BinopExprAST\n";
        }
    };

    ExprAST *logError(const char *msg);
} // namespace underrated
