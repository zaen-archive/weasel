#pragma once

#include "./ast.h"

namespace underrated
{
    std::unique_ptr<ExprAST> parseNumberExpr();
    std::unique_ptr<ExprAST> parseParenExpr();
    std::unique_ptr<ExprAST> parseIdentifierExpr();
    std::unique_ptr<ExprAST> parsePrimary();
    std::unique_ptr<ExprAST> parseExpression();
    std::unique_ptr<ExprAST> parseBinopRhs(int prec, std::unique_ptr<ExprAST> lhs);

    std::unique_ptr<ProtorypeAST> parsePrototype();
    std::unique_ptr<FunctionAST> parseDefinition();
    std::unique_ptr<ProtorypeAST> parseExern();
    std::unique_ptr<FunctionAST> parseTopLevelExpr();
} // namespace Underrated
