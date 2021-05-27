#include "parse/parser.h"
#include "analysis/context.h"

underrated::StatementExpression *underrated::Parser::parseBody()
{
    auto *stmt = new StatementExpression();

    getNextToken(true); // eat '{'
    while (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseCurlyBracket))
    {
        auto *expr = parseExpression();

        if (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseCurlyBracket))
        {
            if (!expr)
            {
                delete stmt;
                return nullptr;
            }

            stmt->addBody(expr);
        }
    }

    getNextToken(); // eat '}'

    return stmt;
}

underrated::Expression *underrated::Parser::parseExpression()
{
    // Literal Expression
    if (getCurrentToken()->isLiteral())
    {
        return parseLiteralExpression();
    }

    // Compound Statement Expression
    if (getCurrentToken()->isKind(TokenKind::TokenDelimOpenCurlyBracket))
    {
        return parseCompoundExpression();
    }

    // Variable Definition Expression
    if (getCurrentToken()->isKeyDefinition())
    {
        return parseDefinitionExpression();
    }

    // Return Expression
    if (getCurrentToken()->isKind(TokenKind::TokenKeyReturn))
    {
        return parseReturnExpression();
    }

    // Group Expression
    if (getCurrentToken()->isKind(TokenKind::TokenDelimOpenParen))
    {
        return nullptr;
    }

    // Ignore New Line
    if (getCurrentToken()->isKind(TokenKind::TokenSpaceNewline))
    {
        getNextToken(true); // Eat 'New Line'

        return parseExpression();
    }

    return nullptr;
}

underrated::Expression *underrated::Parser::parseReturnExpression()
{
    getNextToken(); // eat 'return'
    if (getCurrentToken()->isKind(TokenKind::TokenSpaceNewline))
    {
        getNextToken(); // eat '\n'
        return new ReturnExpression(nullptr);
    }

    auto *expr = parseExpression();
    if (!expr)
    {
        return logError("Expected expression for return statement.");
    }

    return new ReturnExpression(expr);
}

underrated::Expression *underrated::Parser::parseCompoundExpression()
{
    auto *stmt = new StatementExpression();

    getNextToken(true); // eat '{'
    while (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseCurlyBracket))
    {
        auto *expr = parseExpression();
        if (!expr)
        {
            delete stmt;
            return nullptr;
        }

        stmt->addBody(expr);
    }

    getNextToken(true); // eat '}'

    return stmt;
}

// let 'identifier' 'datatype'  = 'expr'
// let 'identifier' 'datatype'
// let 'identifier'             = 'expr'
underrated::Expression *underrated::Parser::parseDefinitionExpression()
{
    auto qualifier = getQualifier();

    getNextToken(); // eat qualifier(let, final, const)
    if (!getCurrentToken()->isKind(TokenKind::TokenIdentifier))
    {
        return logError("Expected an identifier");
    }

    auto identifier = getCurrentToken()->getValue();

    Token *tokenTy = nullptr;
    getNextToken(); // eat 'identifier' and get next token
    if (getCurrentToken()->isDataType())
    {
        tokenTy = getCurrentToken();
        getNextToken(); // eat 'data type'
    }

    if (getCurrentToken()->isKind(TokenKind::TokenSpaceNewline))
    {
        if (!tokenTy)
        {
            return logError("Data Type Expected");
        }

        if (qualifier != Qualifier::QualVolatile)
        {
            return logError("No Default Value for Non Volatile variable");
        }

        // Create Variable with Default Value
        // TODO: Do Default Definition
        // return logError("Default definition called");
        return new AssignmentExpression(new VariableExpression(identifier, true), new NumberLiteralExpression(0));
    }

    // Equal
    // TODO: Should support another type of equal sign
    if (!getCurrentToken()->isKind(TokenKind::TokenPuncEqual))
    {
        return logError("Equal sign expected");
    }

    // Get Value
    getNextToken(); // eat 'Equal Sign'
    if (getCurrentToken()->isKind(TokenKind::TokenSpaceNewline))
    {
        return logError("Expected Value");
    }

    auto *val = parseExpression();

    // TODO: Get Value from expression
    // return logError("Get Value from exression");
    return new AssignmentExpression(new VariableExpression(identifier, true), val);
}

// underrated::Expression *underrated::Parser::parseScalarExpr()
// {
//     return new LiteralExpr(std::stod(this->getCurrentToken()->getValue()));
// }

// underrated::Expression *underrated::Parser::parseExpr()
// {
//     auto *lhs = this->getCurrentToken();
//     if (lhs->getTokenKind() == TokenKind::TokenTyInt)
//     {
//         return parseScalarExpr();
//     }

//     return nullptr;
// }

// underrated::Expression *underrated::Parser::parseVariableExpr()
// {
//     // Eat 'let'
//     auto *token = getNextToken();
//     if (token->getTokenKind() != TokenKind::TokenIdentifier)
//     {
//         return logError("Expecting identifier");
//     }

//     // Eat 'identifier'
//     auto identifier = token->getValue();
//     token = getNextToken();
//     if (token->getTokenKind() == TokenKind::TokenTyInt)
//     {
//         // Our data type just integer
//         // TODO: Just get next token
//         token = getNextToken();

//         // If not define the value yet
//         if (token->getTokenKind() == TokenKind::TokenPuncSemicolon)
//         {
//             return new VariableExpr(identifier, new LiteralExpr(0));
//         }
//     }

//     // Should get assign operator
//     if (token->getTokenKind() != TokenKind::TokenPuncEqual)
//     {
//         return logError("Expecting assign operator");
//     }

//     // Eat 'Assign' Operator
//     getNextToken();

//     // Get An Expression
//     auto *expr = parseExpr();

//     if (!expr)
//     {
//         return logError("Expecting an expression");
//     }

//     if (getNextToken()->getTokenKind() != TokenKind::TokenPuncSemicolon)
//     {
//         return logError("Expecting new line or semicolon");
//     }

//     return new VariableExpr(identifier, expr);
// }
