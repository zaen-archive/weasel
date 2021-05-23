#pragma once

#include <string>
#include <vector>
#include <map>
#include "llvm/IR/Value.h"
#include "ast/type.h"

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
namespace underrated
{
    class AnalysContext;

    class Expression
    {
    public:
        Expression() {}

    public:
        virtual llvm::Value *codegen(AnalysContext *context) = 0;
    };

    class StmtExpression : public Expression
    {
    private:
        std::vector<Expression *> _body;
        // std::map<std::string, Type *> _variables;

    public:
        llvm::Value *codegen(AnalysContext *context) { return nullptr; }
    };

    class LiteralExpression : public Expression
    {
    private:
        int _value;

    public:
        LiteralExpression(int value) : _value(value) {}
        int getValue() const { return _value; }
        llvm::Value *codegen(AnalysContext *context) { return nullptr; }
    };

    class ReturnExpression : public Expression
    {
    public:
        llvm::Value *codegen(AnalysContext *context) { return nullptr; }
    };

    class CallExpression : public Expression
    {
    public:
        llvm::Value *codegen(AnalysContext *context) { return nullptr; }
    };

    /// Operator Expression ///
    class OperatorExpression : public Expression
    {
    };

    // TODO: Debug
    class DebugExpression : public Expression
    {
    public:
        llvm::Value *codegen(AnalysContext *c);
    };
} // namespace underrated
