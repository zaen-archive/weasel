#pragma once

#include <string>
#include <memory>
#include <vector>
#include "./token.h"

namespace underrated
{
    class ExprAST
    {
    };

    class NumberExprAST : public ExprAST
    {
    private:
        double _val;

    public:
        NumberExprAST(double val) : _val(val) {}
    };

    class VariableExprAST : public ExprAST
    {
    private:
        std::string _name;

    public:
        VariableExprAST(const std::string &name) : _name(name) {}
    };

    class BinaryExprAST : public ExprAST
    {
    private:
        TokenKind _op;
        std::unique_ptr<ExprAST> _lhs, _rhs;

    public:
        BinaryExprAST(TokenKind op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs)
            : _op(op), _lhs(std::move(lhs)), _rhs(std::move(rhs)) {}
    };

    class CallExprAST : public ExprAST
    {
    private:
        std::string _calle;
        std::vector<std::unique_ptr<ExprAST>> _args;

    public:
        CallExprAST(std::string &calle, std::vector<std::unique_ptr<ExprAST>> args)
            : _calle(calle), _args(std::move(args)) {}
    };

    // Protorype AST
    // Prototype of the Function
    class ProtorypeAST
    {
    private:
        std::string _name;
        std::vector<std::string> _args;

    public:
        ProtorypeAST(std::string &name, std::vector<std::string> args) : _name(name), _args(move(args)) {}
        std::string &getName() { return _name; }
    };

    class FunctionAST
    {
    private:
        std::unique_ptr<ProtorypeAST> _proto;
        std::unique_ptr<ExprAST> _body;

    public:
        FunctionAST(std::unique_ptr<ProtorypeAST> proto, std::unique_ptr<ExprAST> body)
            : _proto(std::move(proto)), _body(std::move(body)) {}
    };

    // logError - These are little helper functions for error handling.
    std::unique_ptr<ExprAST> logError(const char *msg);

    // logErrorP - Little helper function for prototype
    std::unique_ptr<ProtorypeAST> logErrorP(const char *msg);
} // namespace Underrated
