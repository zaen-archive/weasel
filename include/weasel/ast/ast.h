#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "llvm/IR/Value.h"
#include "weasel/lex/token.h"

///// Expression /////
// VariableExpression
// LiteralExpression
// ArrayLiteralExpression
// | PathExpression
// OperatorExpression
// | GroupedExpression
// ArrayExpression
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
// Declaration of weasel Class
namespace weasel
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
} // namespace weasel

//

//

// Expression Base Type
namespace weasel
{
    // Expression
    class Expression
    {
    protected:
        std::shared_ptr<Token> _token; // Token each expression

    public:
        Expression(std::shared_ptr<Token> token) : _token(token) {}
        Expression() {}

        std::shared_ptr<Token> getToken() const { return _token; }

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
        LiteralExpression(std::shared_ptr<Token> token, LiteralType type, unsigned width, unsigned size = 1) : Expression(token), _type(type), _width(width), _size(size) {}
    };

} // namespace weasel

//

//

// Expression Without Block PART
namespace weasel
{
    // Return Expression
    class ReturnExpression : public Expression
    {
    private:
        std::shared_ptr<Expression> _value;

    public:
        ReturnExpression(std::shared_ptr<Token> token, std::shared_ptr<Expression> value) : Expression(token), _value(value) {}

        std::shared_ptr<Expression> getValue() const { return _value; }

        llvm::Value *codegen(AnalysContext *context) override;
    };

    // Call Expression
    class CallExpression : public Expression
    {
        std::string _identifier;
        std::vector<std::shared_ptr<Expression>> _args;

    public:
        CallExpression(std::shared_ptr<Token> token, std::string identifier, std::vector<std::shared_ptr<Expression>> args) : Expression(token), _identifier(identifier), _args(args) {}

        std::string getIdentifier() const { return _identifier; }
        std::vector<std::shared_ptr<Expression>> getArguments() const { return _args; }

        llvm::Value *codegen(AnalysContext *context) override;
    };

    // Variable Expression
    class VariableExpression : public Expression
    {
    private:
        std::string _identifier;
        bool _addressOf;

    public:
        VariableExpression(std::shared_ptr<Token> token, std::string identifier, bool addressOf = false) : Expression(token), _identifier(identifier), _addressOf(addressOf) {}

        std::string getIdentifier() const { return _identifier; }
        bool isAddressOf() const { return _addressOf; }

        llvm::Value *codegen(AnalysContext *context) override;
    };

    class ArrayExpression : public VariableExpression
    {
    private:
        std::shared_ptr<Expression> _indexExpr;

    public:
        ArrayExpression(std::shared_ptr<Token> token, std::string identifier, std::shared_ptr<Expression> indexExpr, bool addressOf = false) : VariableExpression(token, identifier, addressOf), _indexExpr(indexExpr) {}

        std::shared_ptr<Expression> getIndex() const { return _indexExpr; }

        llvm::Value *codegen(AnalysContext *context) override;
    };

    // Declaration Expression
    class DeclarationExpression : public Expression
    {
    private:
        std::string _identifier;
        Qualifier _qualifier;
        llvm::Type *_type;
        std::shared_ptr<Expression> _value;

    public:
        DeclarationExpression(std::shared_ptr<Token> token, std::string identifier, Qualifier qualifier, llvm::Type *type = nullptr, std::shared_ptr<Expression> value = nullptr) : Expression(token), _identifier(identifier), _qualifier(qualifier), _type(type), _value(value) {}

        std::string getIdentifier() const { return _identifier; }
        std::shared_ptr<Expression> getValue() const { return _value; }
        llvm::Type *getType() const { return _type; }
        Qualifier getQualifier() const { return _qualifier; }

        llvm::Value *codegen(AnalysContext *context) override;
    };

    // Number Literal Expression
    class NumberLiteralExpression : public LiteralExpression
    {
    private:
        long long _value; // 64 bit(8 bytes)

    public:
        NumberLiteralExpression(std::shared_ptr<Token> token, long long value, unsigned width = 32) : LiteralExpression(token, LiteralType::LiteralNumber, width), _value(value) {}

        long long getValue() const { return _value; }

        llvm::Value *codegen(AnalysContext *context) override;
    };

    // Boolean Literal Expression
    class BoolLiteralExpression : public LiteralExpression
    {
    private:
        bool _value;

    public:
        BoolLiteralExpression(std::shared_ptr<Token> token, bool value) : LiteralExpression(token, LiteralType::LiteralBool, 1), _value(value) {}

        bool getValue() const { return _value; }

        llvm::Value *codegen(AnalysContext *context) override { return nullptr; }
    };

    // String Literal Expression
    class StringLiteralExpression : public LiteralExpression
    {
    private:
        std::string _value;

    public:
        StringLiteralExpression(std::shared_ptr<Token> token, std::string value) : LiteralExpression(token, LiteralType::LiteralString, 8, value.size()), _value(value) {}

        std::string getValue() const { return _value; }

        llvm::Value *codegen(AnalysContext *context) override;
    };

    // Nil Literal Expression
    class NilLiteralExpression : public LiteralExpression
    {
    public:
        NilLiteralExpression(std::shared_ptr<Token> token) : LiteralExpression(token, LiteralType::LiteralNil, 64) {}

        llvm::Value *codegen(AnalysContext *context) override;
    };

    // Binary Operator Expression
    class BinaryOperatorExpression : public Expression
    {
    private:
        std::shared_ptr<Token> _operator;
        std::shared_ptr<Expression> _lhs;
        std::shared_ptr<Expression> _rhs;

    public:
        BinaryOperatorExpression(std::shared_ptr<Token> op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs) : _operator(op), _lhs(lhs), _rhs(rhs) {}

        std::shared_ptr<Token> getOperator() const { return _operator; }
        std::shared_ptr<Expression> getLHS() const { return _lhs; }
        std::shared_ptr<Expression> getRHS() const { return _rhs; }

        llvm::Value *codegen(AnalysContext *context) override;
    };

    // Unary Operator Expression
    class UnaryOperatorExpression : public Expression
    {
    private:
        std::shared_ptr<Token> _lhs;
        std::shared_ptr<Expression> _rhs;

    public:
        UnaryOperatorExpression(std::shared_ptr<Token> lhs, std::shared_ptr<Expression> rhs) : _lhs(lhs), _rhs(rhs) {}

        llvm::Value *codegen(AnalysContext *context) override { return nullptr; }
    };

    // Array Expression
    class ArrayLiteralExpression : public Expression
    {
    private:
        std::vector<std::shared_ptr<Expression>> _items;

    public:
        ArrayLiteralExpression() {}
        ArrayLiteralExpression(std::vector<std::shared_ptr<Expression>> items) : _items(items) {}

        void addItem(std::shared_ptr<Expression> item) { _items.push_back(item); }
        std::vector<std::shared_ptr<Expression>> getItems() const { return _items; }

        llvm::Value *codegen(AnalysContext *context) override;
    };

} // namespace weasel

//

//

// Expression With Block Function PART
namespace weasel
{
    // Statement Expression
    class StatementExpression : public Expression
    {
    private:
        std::vector<std::shared_ptr<Expression>> _body;

    public:
        StatementExpression() {}

        void addBody(std::shared_ptr<Expression> expr) { _body.push_back(expr); }
        std::vector<std::shared_ptr<Expression>> getBody() const { return _body; }

        llvm::Value *codegen(AnalysContext *context) override;
    };

} // namespace weasel

//

//

// Function PART
namespace weasel
{
    // Func Arg
    class FunctionArgument
    {
    private:
        std::shared_ptr<Token> _token;
        std::string _argName;
        llvm::Type *_type;

    public:
        FunctionArgument(std::shared_ptr<Token> token, std::string argName, llvm::Type *type) : _token(token), _argName(argName), _type(type) {}

        std::shared_ptr<Token> getToken() const { return _token; }
        llvm::Type *getArgumentType() const { return _type; }
        std::string getArgumentName() const { return _argName; }
    };

    // Func Type
    class FunctionType
    {
    private:
        std::vector<std::shared_ptr<FunctionArgument>> _args;
        llvm::Type *_retType;
        bool _isVararg;

    public:
        FunctionType(llvm::Type *returnType, std::vector<std::shared_ptr<FunctionArgument>> args, bool vararg) : _args(args), _retType(returnType), _isVararg(vararg) {}

        std::vector<std::shared_ptr<FunctionArgument>> getArgs() const { return _args; }
        llvm::Type *getReturnType() const { return _retType; }

        bool getIsVararg() const { return _isVararg; }
    };

    // Func
    class Function
    {
    private:
        std::string _identifier;
        std::shared_ptr<FunctionType> _funcTy;
        std::shared_ptr<StatementExpression> _body;
        bool _isDefine = false;

    public:
        Function(std::string identifier, std::shared_ptr<FunctionType> funcTy) : _identifier(identifier), _funcTy(funcTy) {}

        std::string getIdentifier() const { return _identifier; }
        std::shared_ptr<FunctionType> getFunctionType() const { return _funcTy; }
        std::vector<std::shared_ptr<FunctionArgument>> getArgs() const { return _funcTy->getArgs(); }

        std::shared_ptr<StatementExpression> getBody() const { return _body; }
        void setBody(std::shared_ptr<StatementExpression> body) { _body = body; }

        void setIsDefine(bool val) { _isDefine = val; }
        bool getIsDefine() const { return _isDefine; }

    public:
        llvm::Function *codegen(AnalysContext *c);
    };

} // namespace weasel

//

//

// Log Error for expression and Function
namespace weasel
{
    llvm::Value *logErrorV(std::string msg);
    llvm::Function *logErrorF(std::string msg);

} // namespace weasel
