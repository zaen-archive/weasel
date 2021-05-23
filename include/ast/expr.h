#pragma once

#include <string>
#include "llvm/IR/Value.h"

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

    class LiteralExpr : public Expression
    {
    private:
        int _value;

    public:
        LiteralExpr(int value) : _value(value) {}
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
        llvm::Value *codegen(AnalysContext *context) { return nullptr; }
    };
} // namespace underrated
