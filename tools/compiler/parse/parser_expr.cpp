#include "zero/parse/parser.h"
#include "zero/analysis/context.h"
#include "zero/symbol/symbol.h"

void zero::Parser::ignoreNewline()
{
    if (getCurrentToken()->isNewline())
    {
        getNextToken(true);
    }
}

zero::StatementExpression *zero::Parser::parseFunctionBody()
{
    auto *stmt = new StatementExpression();

    // Enter statement scope
    {
        SymbolTable::getInstance().enterScope();
    }

    getNextToken(true); // eat '{'
    while (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseCurlyBracket))
    {
        auto *expr = parseStatement();

        if (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseCurlyBracket))
        {
            if (expr)
            {
                stmt->addBody(expr);
            }
        }

        ignoreNewline();
    }
    getNextToken(); // eat '}'

    // Exit statement scope
    {
        SymbolTable::getInstance().exitScope();
    }

    return stmt;
}

zero::Expression *zero::Parser::parseStatement()
{
    // Compound Statement Expression
    if (getCurrentToken()->isKind(TokenKind::TokenDelimOpenCurlyBracket))
    {
        return parseCompoundStatement();
    }

    // Variable Definition Expression
    if (getCurrentToken()->isKeyDefinition())
    {
        return parseDeclarationExpression();
    }

    // Return Expression
    if (getCurrentToken()->isKind(TokenKind::TokenKeyReturn))
    {
        return parseReturnStatement();
    }

    auto *expr = parseExpression();
    if (!expr)
    {
        auto *err = new Error(getCurrentToken(), "Invalid expression statement");

        getNextTokenUntil(TokenKind::TokenSpaceNewline);

        return ErrorTable::addError(err);
    }

    return expr;
}

zero::Expression *zero::Parser::parseLiteralExpression()
{
    auto *token = getCurrentToken();
    if (token->isKind(TokenKind::TokenLitBool))
    {
        return new BoolLiteralExpression(token, token->getValue() == "true");
    }

    if (token->isKind(TokenKind::TokenLitChar))
    {
        auto val = std::stoi(token->getValue());
        return new NumberLiteralExpression(token, val, 8);
    }

    if (token->isKind(TokenKind::TokenLitNumber))
    {
        auto value = strtoll(token->getValue().c_str(), 0, 10);
        return new NumberLiteralExpression(token, value);
    }

    if (token->isKind(TokenKind::TokenLitString))
    {
        return new StringLiteralExpression(token, token->getValue());
    }

    return new NilLiteralExpression();
}

zero::Expression *zero::Parser::parseFunctionCallExpression(zero::Attribute *attr)
{
    auto identifier = getCurrentToken()->getValue();
    getNextToken();

    std::vector<Expression *> args;
    if (!getNextToken()->isKind(TokenKind::TokenDelimCloseParen))
    {
        while (true)
        {
            if (auto *arg = parseExpression())
            {
                args.push_back(arg);
            }
            else
            {
                return ErrorTable::addError(new Error(getCurrentToken(), "Expected argument expression"));
            }

            if (getCurrentToken()->isKind(TokenKind::TokenDelimCloseParen))
            {
                break;
            }

            if (!getCurrentToken()->isKind(TokenKind::TokenPuncComma))
            {
                return ErrorTable::addError(new Error(getCurrentToken(), "Expected ) or , in argument list"));
            }

            getNextToken();
        }
    }

    getNextToken(); // eat )
    return new CallExpression(identifier, args);
}

zero::Expression *zero::Parser::parseIdentifierExpression()
{
    auto identifier = getCurrentToken()->getValue();

    // Check Variable exit
    auto *attr = SymbolTable::getInstance().get(identifier);
    if (!attr)
    {
        return ErrorTable::addError(new Error(getCurrentToken(), "Variable not yet declared"));
    }

    if (attr->getKind() == AttributeKind::SymbolFunction)
    {
        return parseFunctionCallExpression(attr);
    }

    return new VariableExpression(getCurrentToken(), identifier);
}

// TODO: Remove cout
zero::Expression *zero::Parser::parseParenExpression()
{
    getNextToken(); // eat (
    auto *expr = parseExpression();
    if (!expr)
    {
        return ErrorTable::addError(getCurrentToken(), "Expected expression inside after (..");
    }
    auto *val = (BinaryOperatorExpression *)expr;
    if (val)
    {
        auto *lhs = (NumberLiteralExpression *)(val->getLHS());
        auto *rhs = (NumberLiteralExpression *)(val->getRHS());

        std::cout << lhs->getValue() << " < " << val->getToken() << " > " << rhs->getValue() << "\n";
    }
    else
    {
        std::cout << "YOU STU\n";
    }

    if (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseParen))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected )");
    }

    getNextToken(); // eat )
    return expr;
}

zero::Expression *zero::Parser::parsePrimaryExpression()
{
    if (getCurrentToken()->isLiteral())
    {
        return parseLiteralExpression();
    }

    if (getCurrentToken()->isKind(TokenKind::TokenIdentifier))
    {
        return parseIdentifierExpression();
    }

    if (getCurrentToken()->isKind(TokenKind::TokenDelimOpenParen))
    {
        return parseParenExpression();
    }

    return ErrorTable::addError(getCurrentToken(), "Expected expression");
}

zero::Expression *zero::Parser::parseExpression()
{
    auto *lhs = parsePrimaryExpression();
    if (!lhs)
    {
        return ErrorTable::addError(new Error(getCurrentToken(), "Expected LHS"));
    }

    getNextToken(); // Eat 'LHS' Expression
    return parseBinaryOperator(zero::defPrecOrder, lhs);
}

zero::Expression *zero::Parser::parseBinaryOperator(unsigned precOrder, zero::Expression *lhs)
{
    while (true)
    {
        // precedence =
        auto *binOp = getCurrentToken();
        if (!binOp->isOperator() || binOp->isNewline())
        {
            return lhs;
        }

        auto prec = binOp->getPrecedence();
        if (prec.order > precOrder)
        {
            return lhs;
        }

        getNextToken(); // eat 'operator'
        // eat =
        auto *rhs = parsePrimaryExpression();
        if (!rhs)
        {
            return ErrorTable::addError(new Error(getCurrentToken(), "Expected RHS Expression 1"));
        }

        getNextToken(); // eat 'rhs'
        rhs = parseBinaryOperator(prec.order, rhs);
        if (!rhs)
        {
            return ErrorTable::addError(new Error(getCurrentToken(), "Expected RHS Expression 2"));
        }

        lhs = new BinaryOperatorExpression(binOp, lhs, rhs);
    }
}

zero::Expression *zero::Parser::parseReturnStatement()
{
    auto *retToken = getCurrentToken();
    getNextToken(); // eat 'return'
    auto *expr = parseExpression();
    if (!expr)
    {
        auto *err = new Error(getCurrentToken(), "Expected expression for return statement.");

        getNextTokenUntil(TokenKind::TokenSpaceNewline);

        return ErrorTable::addError(err);
    }

    return new ReturnExpression(retToken, expr);
}

zero::Expression *zero::Parser::parseCompoundStatement()
{
    auto *stmt = new StatementExpression();

    // Enter Statement Scope
    {
        SymbolTable::getInstance().enterScope();
    }

    getNextToken(true); // eat '{'
    while (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseCurlyBracket))
    {
        auto *expr = parseExpression();
        if (!expr)
        {
            return ErrorTable::addError(getCurrentToken(), "Expected statement");
        }

        stmt->addBody(expr);
    }
    getNextToken(true); // eat '}'

    // Exit Statement Scope
    {
        SymbolTable::getInstance().exitScope();
    }

    return stmt;
}

// let 'identifier' 'datatype'  = 'expr'
// let 'identifier' 'datatype'
// let 'identifier'             = 'expr'
zero::Expression *zero::Parser::parseDeclarationExpression()
{
    auto qualifier = getQualifier();
    auto *qualToken = getCurrentToken();

    getNextToken(); // eat qualifier(let, final, const)
    if (!getCurrentToken()->isKind(TokenKind::TokenIdentifier))
    {
        auto *errToken = getCurrentToken();

        getNextTokenUntil(TokenKind::TokenSpaceNewline);
        return ErrorTable::addError(errToken, "Expected an identifier");
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
            return ErrorTable::addError(getCurrentToken(), "Data Type Expected for default value declaration");
        }

        if (qualifier != Qualifier::QualVolatile)
        {
            return ErrorTable::addError(getCurrentToken(), "No Default Value for Non Volatile variable");
        }
        auto *type = tokenTy->toType(getContext());

        // Insert Symbol Table
        {
            auto *attr = new Attribute(identifier, AttributeScope::ScopeLocal, AttributeKind::SymbolVariable, type);
            SymbolTable::getInstance().insert(identifier, attr);
        }

        // Create Variable with Default Value
        return new DeclarationExpression(qualToken, identifier, qualifier, type);
    }

    // Equal
    if (!getCurrentToken()->isKind(TokenKind::TokenOperatorEqual))
    {
        auto *errToken = getCurrentToken();

        getNextTokenUntil(TokenKind::TokenSpaceNewline);
        return ErrorTable::addError(errToken, "Expected equal sign");
    }

    // Get Value
    getNextToken(); // eat 'Equal Sign'
    if (getCurrentToken()->isKind(TokenKind::TokenSpaceNewline))
    {
        return ErrorTable::addError(new Error(getCurrentToken(), "Expected RHS Value Expression but got 'New line'"));
    }

    auto *val = parseExpression();
    if (!val)
    {
        auto *err = new Error(getCurrentToken(), "Expected RHS Value Expression but got not valid expression");

        getNextTokenUntil(TokenKind::TokenSpaceNewline);

        return ErrorTable::addError(err);
    }

    llvm::Type *type = nullptr;
    if (tokenTy)
    {
        type = tokenTy->toType(getContext());
    }

    // Insert Symbol Table
    {
        auto *attr = new Attribute(identifier, AttributeScope::ScopeLocal, AttributeKind::SymbolVariable, type);
        SymbolTable::getInstance().insert(identifier, attr);
    }

    return new DeclarationExpression(qualToken, identifier, qualifier, type, val);
}
