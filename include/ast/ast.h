#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "llvm/IR/Value.h"
#include "lex/token.h"

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
// Declaration of underrated Class
namespace underrated
{
    // Analysis Context
    class AnalysContext;

    // Literal Type
    enum class LiteralType
    {
        LiteralNil,
        LiteralBool,
        LiteralNumber,
        LiteralChar,
        LiteralString,
    };

    // Type
    class Type;

    // Package
    class Package;
    class ErrorExpression;

    // Function
    class FunctionArgument;
    class FunctionType;
    class Function;

    // Base Expression
    class Expression;
    class BlockExpression;

    // Expression Without Block
    class AssignmentExpression;
    class LiteralExpression;
    class ReturnExpression;
    class CallExpression;
    class VariableExpression;
    class UnaryOperatorExpression;

    // Expression With Block
    class StatementExpression;
} // namespace underrated

//

//

// Expression Base Type
namespace underrated
{
    // Type
    class Type
    {
    private:
        llvm::Type *_llvmType;
        Qualifier _qual;
        bool _isSigned;

    public:
        Type(llvm::Type *llvmType, Qualifier qualifier = Qualifier::QualVolatile, bool isSigned = true) : _llvmType(llvmType), _qual(qualifier), _isSigned(isSigned) {}

        llvm::Type *getLLVMType() const { return _llvmType; }
    };

    // Expression
    class Expression
    {
    public:
        Expression() {}

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
        LiteralExpression(LiteralType type, unsigned width, unsigned size = 1) : _type(type), _width(width), _size(size) {}
    };

    // Block Expression
    class BlockExpression : public Expression
    {
    protected:
        std::map<std::string, Type *> _variable;
        BlockExpression *_parent;

    public:
        BlockExpression(BlockExpression *parent) : _parent(parent) {}

        BlockExpression *getParent() const { return _parent; }
        Type *getVariable(std::string name);
    };

    // TODO: Debug Expression
    class DebugExpression : public Expression
    {
    public:
        llvm::Value *codegen(AnalysContext *c);
    };
} // namespace underrated

//

//

// Expression Without Block PART
namespace underrated
{
    // Assignment Expression
    class AssignmentExpression : public Expression
    {
    private:
        Expression *_lhs;
        Expression *_rhs;

    public:
        AssignmentExpression(Expression *lhs, Expression *rhs) : _lhs(lhs), _rhs(rhs) {}

        Expression *getLHS() const { return _lhs; }
        Expression *getRHS() const { return _rhs; }
        llvm::Value *codegen(AnalysContext *context);
    };

    // Return Expression
    class ReturnExpression : public Expression
    {
    private:
        Expression *_value;

    public:
        ReturnExpression(Expression *value) : _value(value) {}

        Expression *getValue() const { return _value; }
        llvm::Value *codegen(AnalysContext *context);
    };

    // Call Expression
    class CallExpression : public Expression
    {
    public:
        llvm::Value *codegen(AnalysContext *context) { return nullptr; }
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
        VariableExpression(std::string identifier, bool declare = false) : _identifier(identifier), _declare(declare) {}

        std::string getIdentifier() const { return _identifier; }

        llvm::Value *codegen(AnalysContext *context);
    };

    // Number Literal Expression
    class NumberLiteralExpression : public LiteralExpression
    {
    private:
        long long _value; // 64 bit(8 bytes)

    public:
        NumberLiteralExpression(long long value, unsigned width = 32) : _value(value), LiteralExpression(LiteralType::LiteralNumber, width) {}

        long long getValue() const { return _value; }

        llvm::Value *codegen(AnalysContext *context);
    };

    // Boolean Literal Expression
    class BoolLiteralExpression : public LiteralExpression
    {
    private:
        bool _value;

    public:
        BoolLiteralExpression(bool value) : _value(value), LiteralExpression(LiteralType::LiteralBool, 1) {}

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
        StringLiteralExpression(std::string value) : _value(value), LiteralExpression(LiteralType::LiteralString, 8, value.size()) {}

        std::string getValue() const { return _value; }

        llvm::Value *codegen(AnalysContext *context)
        {
            return nullptr;
        }
    };

    class NilLiteralExpression : public LiteralExpression
    {
    public:
        NilLiteralExpression() : LiteralExpression(LiteralType::LiteralNil, 64) {}

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

} // namespace underrated

//

//

// Expression With Block Function PART
namespace underrated
{
    // TODO: The StatementExpression should link to past Statement Expression
    // Statement Expression
    class StatementExpression : public BlockExpression
    {
    private:
        std::vector<Expression *> _body;
        Function *_func;

    public:
        StatementExpression() : BlockExpression(nullptr) {}

        void addBody(Expression *expr) { _body.push_back(expr); }
        std::vector<Expression *> getBody() const { return _body; }

        // void setFunction(Func *func) { _func = func; }
        Function *getFunction() const { return _func; }

        llvm::Value *codegen(AnalysContext *context);
    };

} // namespace underrated

//

//

// TODO: Just support sign operation
// Function PART
namespace underrated
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

} // namespace underrated

//

//

namespace underrated
{
    class ErrorExpression : public Expression
    {
    private:
        std::vector<Token *> _tokens;

    public:
        llvm::Value *codegen(AnalysContext *c) { return nullptr; }
    };

    class Package
    {
    private:
        std::vector<Function *> _functions;

    public:
        std::vector<ErrorExpression *> codegen(AnalysContext *c) { return {}; }
    };

} // namespace underrated

//

//

// Log Error for expression and Function
namespace underrated
{
    Expression *logError(const char *msg);
    llvm::Value *logErrorV(const char *msg);
    Function *logErrorF(const char *msg);

} // namespace underrated
