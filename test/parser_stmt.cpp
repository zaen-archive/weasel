#include <vector>
#include "parse/parser.h"

std::vector<underrated::Stmt *> underrated::Parser::parseBody()
{
    std::vector<underrated::Stmt *> body;

    // Compound Statement
    Stmt *stmt;

    while (true)
    {
        // If got '}' return the body
        if (getCurrentToken()->isKind(TokenKind::TokenCloseBracket))
        {
            getNextToken(); // eat '}'
            return body;
        }
        // '{' statements '}'
        else if (getCurrentToken()->isKind(TokenKind::TokenOpenBracket))
        {
            stmt = parseCompoundStatement();
        }
        // 'if' exprs '{' stmt '}'
        else if (getCurrentToken()->isKind(TokenKind::TokenCondIf))
        {
            stmt = parseCondtionalStatement();
        }
        // 'for' exprs '{' stmt '}'
        else if (getCurrentToken()->isKind(TokenKind::TokenLoopFor))
        {
            stmt = parseLoopStatement();
        }
        // 'return' exprs
        else if (getCurrentToken()->isKind(TokenKind::TokenKeyReturn))
        {
            stmt = parseReturnStatement();
        }
        // 'let' varDef1
        else if (
            getCurrentToken()->isKind(TokenKind::TokenKeyLet) ||
            getCurrentToken()->isKind(TokenKind::TokenKeyFinal) ||
            getCurrentToken()->isKind(TokenKind::TokenKeyConst))
        {
            stmt = parseVariableStatement();
        }
        else
        {
            stmt = parseExpression();
        }

        body.push_back(stmt);

        if (!stmt)
        {
            return body;
        }
        else
        {
            stmt = nullptr;
        }
    }

    return body;
}

underrated::Stmt *underrated::Parser::parseVariableStatement()
{
    // TODO: Need to save current qualifier
    getNextToken(); // eat 'let'

    if (!getCurrentToken()->isKind(TokenKind::TokenIdentifier))
    {
        return nullptr;
    }

    auto identifier = getCurrentToken()->getValue();

    getNextToken(); // eat 'identifier'

    // TODO: Need to save current data type
    auto *ty = getCurrentToken();
    if (getCurrentToken()->isDataType())
    {
        getNextToken(); // eat 'data type'
    }

    // New Line mean just declare variable
    if (getCurrentToken()->isKind(TokenKind::TokenNewline))
    {
        return nullptr;
    }

    // TODO: Only support assign operator
    // need to expand
    if (!getCurrentToken()->isKind(TokenKind::TokenOpAssign))
    {
        return nullptr;
    }

    getNextToken(); // eat '='

    auto *expr = parseExpression();

    return nullptr;
}
