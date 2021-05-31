#include "parse/parser.h"
#include "analysis/context.h"

void underrated::Parser::ignoreNewline()
{
    if (getCurrentToken()->isNewline())
    {
        getNextToken(true);
    }
}

underrated::StatementExpression *underrated::Parser::parseFunctionBody()
{
    auto *stmt = new StatementExpression();

    getNextToken(true); // eat '{'
    while (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseCurlyBracket))
    {
        auto *expr = parseStatement();

        if (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseCurlyBracket))
        {
            if (!expr)
            {
                delete stmt;
                return nullptr;
            }

            stmt->addBody(expr);
        }

        ignoreNewline();
    }

    getNextToken(); // eat '}'
    return stmt;
}

underrated::Expression *underrated::Parser::parseStatement()
{
    // Compound Statement Expression
    if (getCurrentToken()->isKind(TokenKind::TokenDelimOpenCurlyBracket))
    {
        return parseCompoundStatement();
    }

    // Variable Definition Expression
    if (getCurrentToken()->isKeyDefinition())
    {
        return parseDefinitionExpression();
    }

    // Return Expression
    if (getCurrentToken()->isKind(TokenKind::TokenKeyReturn))
    {
        return parseReturnStatement();
    }

    return parseExpression();
}

underrated::Expression *underrated::Parser::parseLiteralExpression()
{
    auto *token = getCurrentToken();
    if (token->isKind(TokenKind::TokenLitBool))
    {
        return new BoolLiteralExpression(token->getValue() == "true");
    }

    if (token->isKind(TokenKind::TokenLitChar))
    {
        auto val = std::stoi(token->getValue());
        return new NumberLiteralExpression(val, 8);
    }

    if (token->isKind(TokenKind::TokenLitNumber))
    {
        auto value = strtoll(token->getValue().c_str(), 0, 10);
        return new NumberLiteralExpression(value);
    }

    if (token->isKind(TokenKind::TokenLitString))
    {
        return new StringLiteralExpression(token->getValue());
    }

    return new NilLiteralExpression();
}

underrated::Expression *underrated::Parser::parseVariableExpression()
{
    return new VariableExpression(getCurrentToken()->getValue());
}

underrated::Expression *underrated::Parser::parsePrimaryExpression()
{
    if (getCurrentToken()->isLiteral())
    {
        return parseLiteralExpression();
    }

    if (getCurrentToken()->isKind(TokenKind::TokenIdentifier))
    {
        return parseVariableExpression();
    }

    return nullptr;
}

underrated::Expression *underrated::Parser::parseExpression()
{
    auto *lhs = parsePrimaryExpression();
    if (!lhs)
    {
        return logError("Expected LHS Expression");
    }

    getNextToken(); // Eat 'LHS' Expression
    return parseBinaryOperator(underrated::defPrecOrder, lhs);
}

underrated::Expression *underrated::Parser::parseBinaryOperator(int precOrder, underrated::Expression *lhs)
{
    while (true)
    {
        auto *binOp = getCurrentToken();
        if (!binOp->isOperator())
        {
            return lhs;
        }

        auto prec = binOp->getPrecedence();
        if (prec.order > precOrder)
        {
            return lhs;
        }

        getNextToken(); // eat 'operator'
        auto *rhs = parsePrimaryExpression();
        if (!rhs)
        {
            return logError("Expected RHS Expression");
        }

        getNextToken(); // eat 'rhs'

        rhs = parseBinaryOperator(prec.order, rhs);
        if (!rhs)
        {
            return nullptr;
        }

        lhs = new BinaryOperatorExpression(binOp, lhs, rhs);
    }
}

underrated::Expression *underrated::Parser::parseReturnStatement()
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

underrated::Expression *underrated::Parser::parseCompoundStatement()
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
        // TODO: Support wide default data type
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
    if (!val)
    {
        return logError("Expected Value Expression");
    }

    return new AssignmentExpression(new VariableExpression(identifier, true), val);
}
