#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "llvm/IR/Value.h"
#include "zero/lex/token.h"

///// Expression /////
// VariableExpression
// LiteralExpression
// | PathExpression
// OperatorExpression
// | GroupedExpression
// | ArrayExpression
// | AwaitExpression
// | IndexExpression
// | TupleExpression
// | TupleIndexingExpression
// | StructExpression
// CallExpression
// | MethodCallExpression
// | FieldExpression
// | ClosureExpression
// | ContinueExpression
// | BreakExpression
// | RangeExpression
// ReturnExpression

/// Operator Expression ///
// UnaryOperatorExpression
// | BorrowExpression
// | DereferenceExpression
// | ErrorPropagationExpression
// | NegationExpression
// | ArithmeticOrLogicalExpression
// | ComparisonExpression
// | LazyBooleanExpression
// | TypeCastExpression
// | AssignmentExpression
// | CompoundAssignmentExpression
// Declaration of zero Class
namespace zero
{
    // Analysis Context
    class AnalysContext;

    // Type
    class Type;

    // Package
    class Package;

    // Function
    class FunctionArgument;
    class FunctionType;
    class Function;

    // Base Expression
    class Expression;

    // Expression Without Block
    class LiteralExpression;
    class ReturnExpression;
    class CallExpression;
    class VariableExpression;
    class UnaryOperatorExpression;
    class DeclarationExpression;

    // Expression With Block
    class StatementExpression;

    // Literal Type
    enum class LiteralType
    {
        LiteralNil,
        LiteralBool,
        LiteralNumber,
        LiteralChar,
        LiteralString,
    };
} // namespace zero

//

//

// Expression Base Type
namespace zero
{
    // Expression
    class Expression
    {
    protected:
        Token *_token; // Token each expression

    public:
        Expression(Token *token) : _token(token) {}
        Expression() {}

        Token *getToken() const { return _token; }

        virtual llvm::Value *codegen(AnalysContext *context) = 0;
    };

    // Literal Expression
    class LiteralExpression : public Expression
    {
    protected:
        LiteralType _type;
        unsigned _width;
        unsigned _size;

    public:
        LiteralExpression(Token *token, LiteralType type, unsigned width, unsigned size = 1) : Expression(token), _type(type), _width(width), _size(size) {}
    };

} // namespace zero

//

//

// Expression Without Block PART
namespace zero
{
    // Return Expression
    class ReturnExpression : public Expression
    {
    private:
        Expression *_value;

    public:
        ReturnExpression(Token *token, Expression *value) : Expression(token), _value(value) {}

        Expression *getValue() const { return _value; }

        llvm::Value *codegen(AnalysContext *context);
    };

    // Call Expression
    class CallExpression : public Expression
    {
        std::string _identifier;
        std::vector<Expression *> _args;

    public:
        CallExpression(std::string identifier, std::vector<Expression *> args) : _identifier(identifier), _args(args) {}

        std::string getIdentifier() const { return _identifier; }
        std::vector<Expression *> getArguments() const { return _args; }

        llvm::Value *codegen(AnalysContext *context);
    };

    // Group Expression
    class GroupExpression : public Expression
    {
    public:
        llvm::Value *codegen(AnalysContext *context) { return nullptr; }
    };

    // Variable Expression
    class VariableExpression : public Expression
    {
    private:
        std::string _identifier;
        bool _declare;

    public:
        VariableExpression(Token *token, std::string identifier, bool declare = false) : Expression(token), _identifier(identifier), _declare(declare) {}

        std::string getIdentifier() const { return _identifier; }

        llvm::Value *codegen(AnalysContext *context);
    };

    // Declaration Expression
    class DeclarationExpression : public Expression
    {
    private:
        std::string _identifier;
        llvm::Type *_type;
        Expression *_value;
        Qualifier _qualifier;

    public:
        DeclarationExpression(Token *token, std::string identifier, Qualifier qualifier, llvm::Type *type = nullptr, Expression *value = nullptr) : Expression(token), _identifier(identifier), _type(type), _qualifier(qualifier), _value(value) {}

        std::string getIdentifier() const { return _identifier; }
        llvm::Type *getType() const { return _type; }
        Expression *getValue() const { return _value; }
        Qualifier getQualifier() const { return _qualifier; }

        llvm::Value *codegen(AnalysContext *context);
    };

    // Number Literal Expression
    class NumberLiteralExpression : public LiteralExpression
    {
    private:
        long long _value; // 64 bit(8 bytes)

    public:
        NumberLiteralExpression(Token *token, long long value, unsigned width = 32) : _value(value), LiteralExpression(token, LiteralType::LiteralNumber, width) {}

        long long getValue() const { return _value; }

        llvm::Value *codegen(AnalysContext *context);
    };

    // Boolean Literal Expression
    class BoolLiteralExpression : public LiteralExpression
    {
    private:
        bool _value;

    public:
        BoolLiteralExpression(Token *token, bool value) : _value(value), LiteralExpression(token, LiteralType::LiteralBool, 1) {}

        bool getValue() const { return _value; }

        llvm::Value *codegen(AnalysContext *context)
        {
            return nullptr;
        }
    };

    // String Literal Expression
    class StringLiteralExpression : public LiteralExpression
    {
    private:
        std::string _value;

    public:
        StringLiteralExpression(Token *token, std::string value) : _value(value), LiteralExpression(token, LiteralType::LiteralString, 8, value.size()) {}

        std::string getValue() const { return _value; }

        llvm::Value *codegen(AnalysContext *context)
        {
            return nullptr;
        }
    };

    // Nil Literal Expression
    class NilLiteralExpression : public LiteralExpression
    {
    public:
        NilLiteralExpression() : LiteralExpression(nullptr, LiteralType::LiteralNil, 64) {}

        llvm::Value *codegen(AnalysContext *context)
        {
            return nullptr;
        }
    };

    // Binary Operator Expression
    class BinaryOperatorExpression : public Expression
    {
    private:
        Token *_operator;
        Expression *_lhs;
        Expression *_rhs;

    public:
        BinaryOperatorExpression(Token *op, Expression *lhs, Expression *rhs) : _operator(op), _lhs(lhs), _rhs(rhs) {}

        Token *getOperator() const { return _operator; }
        Expression *getLHS() const { return _lhs; }
        Expression *getRHS() const { return _rhs; }

        llvm::Value *codegen(AnalysContext *context);
    };

    // Unary Operator Expression
    class UnaryOperatorExpression : public Expression
    {
    private:
        Token *_lhs;
        Expression *_rhs;

    public:
        UnaryOperatorExpression(Token *lhs, Expression *rhs) : _lhs(lhs), _rhs(rhs) {}

        llvm::Value *codegen(AnalysContext *context)
        {
            return nullptr;
        }
    };

} // namespace zero

//

//

// Expression With Block Function PART
namespace zero
{
    // Statement Expression
    class StatementExpression : public Expression
    {
    private:
        std::vector<Expression *> _body;
        Function *_func;

    public:
        StatementExpression() {}

        void addBody(Expression *expr) { _body.push_back(expr); }
        std::vector<Expression *> getBody() const { return _body; }

        // void setFunction(Func *func) { _func = func; }
        Function *getFunction() const { return _func; }

        llvm::Value *codegen(AnalysContext *context);
    };

} // namespace zero

//

//

// Function PART
namespace zero
{
    // Func Arg
    class FunctionArgument
    {
    private:
        std::string _argName;
        llvm::Type *_type;

    public:
        FunctionArgument(std::string argName, llvm::Type *type) : _argName(argName), _type(type) {}

        llvm::Type *getArgumentType() const { return _type; }
        std::string getArgumentName() const { return _argName; }
    };

    // Func Type
    class FunctionType
    {
    private:
        std::vector<FunctionArgument *> _args;
        llvm::Type *_retType;

    public:
        FunctionType(llvm::Type *returnType, std::vector<FunctionArgument *> args) : _retType(returnType), _args(args) {}

        std::vector<FunctionArgument *> getArgs() const { return _args; }
        llvm::Type *getReturnType() const { return _retType; }
    };

    // Func
    class Function
    {
    private:
        std::string _identifier;
        FunctionType *_funcTy;
        StatementExpression *_body;
        bool _isDefine = false;

    public:
        Function(std::string identifier, FunctionType *funcTy) : _identifier(identifier), _funcTy(funcTy) {}

        std::string getIdentifier() const { return _identifier; }
        FunctionType *getFunctionType() const { return _funcTy; }
        std::vector<FunctionArgument *> getArgs() const { return _funcTy->getArgs(); }

        StatementExpression *getBody() const { return _body; }
        void setBody(StatementExpression *body);

        void setIsDefine(bool val) { _isDefine = val; }
        bool getIsDefine() const { return _isDefine; }

    public:
        llvm::Function *codegen(AnalysContext *c);
    };

} // namespace zero

//

//

// Log Error for expression and Function
namespace zero
{
    llvm::Value *logErrorV(std::string &msg);
    Function *logErrorF(std::string &msg);

} // namespace zero
