#include "parse/parser.h"
#include <iostream>

// underrated::ExprAST *underrated::Parser::parseExpr()
// {
//     auto *lhs = _lexer->getCurrentToken();
//     auto tokenKind = lhs->getTokenKind();

//     _lexer->getNextToken();

//     // Number Token
//     if (tokenKind == TokenKind::TokenNumber)
//     {
//         return new NumberExprAST(lhs);
//     }

//     // Identifier
//     if (tokenKind == TokenKind::TokenIdentifier)
//     {
//         return new VariableExprAST(lhs);
//     }

//     // Parentise
//     if (tokenKind == TokenKind::TokenOpenParen)
//     {
//         // parseBinOp
//     }

//     // Unary Expression

//     // Error Expression
//     return logError("Unknown token when expecting an expression");
// }

underrated::ExprAST *underrated::Parser::parseNumberExpr()
{
    return new NumberExprAST(std::stod(this->getCurrentToken()->getValue()));
}

underrated::ExprAST *underrated::Parser::parseExpr()
{
    auto *lhs = this->getCurrentToken();
    if (lhs->getTokenKind() == TokenKind::TokenNumber)
    {
        return parseNumberExpr();
    }

    return nullptr;
}

// underrated::ExprAST *underrated::Parser::parseBinopExpr()
// {
// }

underrated::ExprAST *underrated::Parser::parseVariableExpr()
{
    // Eat 'let'
    auto *token = getNextToken();
    if (token->getTokenKind() != TokenKind::TokenIdentifier)
    {
        return logError("Expecting identifier");
    }

    // Eat 'identifier'
    auto identifier = token->getValue();
    token = getNextToken();
    if (token->getTokenKind() == TokenKind::TokenKeyInt)
    {
        // Our data type just integer
        // TODO: Just get next token
        token = getNextToken();

        // If not define the value yet
        if (token->getTokenKind() == TokenKind::TokenSemicolon)
        {
            return new VariableExprAST(identifier, new NumberExprAST(0));
        }
    }

    // Should get assign operator
    if (token->getTokenKind() != TokenKind::TokenOpAssign)
    {
        return logError("Expecting assign operator");
    }

    // Eat 'Assign' Operator
    getNextToken();

    // Get An Expression
    auto *expr = parseExpr();

    if (!expr)
    {
        return logError("Expecting an expression");
    }

    if (getNextToken()->getTokenKind() != TokenKind::TokenSemicolon)
    {
        return logError("Expecting new line or semicolon");
    }

    return new VariableExprAST(identifier, expr);
}

underrated::ExprAST *underrated::Parser::parse()
{
    auto *token = getNextToken();

    if (token->getTokenKind() == TokenKind::TokenDebug)
    {
        getNextToken();
        return new DebugExprAST();
    }

    if (token->getTokenKind() == TokenKind::TokenKeyLet)
    {
        return this->parseVariableExpr();
    }

    return logError("Expecting new code here!");
}
