#include "weasel/parse/parser.h"
#include "weasel/ir/context.h"
#include "weasel/symbol/symbol.h"

std::shared_ptr<weasel::StatementExpression> weasel::Parser::parseFunctionBody()
{
    auto stmt = std::make_shared<StatementExpression>();

    // Enter statement scope
    {
        SymbolTable::enterScope();
    }

    getNextToken(true); // eat '{'
    while (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseCurlyBracket))
    {
        auto expr = parseStatement();

        if (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseCurlyBracket))
        {
            if (expr)
            {
                stmt->addBody(expr);
            }
            else
            {
                ErrorTable::addError(getCurrentToken(), "Expected statement");
            }
        }

        ignoreNewline();
    }
    getNextToken(); // eat '}'

    // Exit statement scope
    {
        SymbolTable::exitScope();
    }

    return stmt;
}

std::shared_ptr<weasel::Expression> weasel::Parser::parseStatement()
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

    auto expr = parseExpression();
    if (!expr)
    {
        auto errToken = getCurrentToken();

        getNextTokenUntil(TokenKind::TokenSpaceNewline);
        return ErrorTable::addError(errToken, "Invalid expression statement");
    }

    return expr;
}

std::shared_ptr<weasel::Expression> weasel::Parser::parseLiteralExpression()
{
    auto token = getCurrentToken();
    if (token->isKind(TokenKind::TokenLitBool))
    {
        return std::make_shared<BoolLiteralExpression>(token, token->getValue() == "true");
    }

    if (token->isKind(TokenKind::TokenLitChar))
    {
        auto val = std::stoi(token->getValue());
        return std::make_shared<NumberLiteralExpression>(token, val, 8);
    }

    if (token->isKind(TokenKind::TokenLitNumber))
    {
        auto value = strtoll(token->getValue().c_str(), nullptr, 10);
        return std::make_shared<NumberLiteralExpression>(token, value);
    }

    if (token->isKind(TokenKind::TokenLitString))
    {
        return std::make_shared<StringLiteralExpression>(token, token->getValue());
    }

    return std::make_shared<NilLiteralExpression>(getCurrentToken());
}

std::shared_ptr<weasel::Expression> weasel::Parser::parseFunctionCallExpression(const std::shared_ptr<weasel::Attribute>& attr)
{
    auto callToken = getCurrentToken();
    if (!getNextToken()->isKind(TokenKind::TokenDelimOpenParen))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected ( for function call");
    }

    std::vector<std::shared_ptr<Expression>> args;
    if (!getNextToken()->isKind(TokenKind::TokenDelimCloseParen))
    {
        while (true)
        {
            if (auto arg = parseExpression())
            {
                args.push_back(arg);
            }
            else
            {
                return ErrorTable::addError(getCurrentToken(), "Expected argument expression");
            }

            if (getCurrentToken()->isKind(TokenKind::TokenDelimCloseParen))
            {
                break;
            }

            if (!getCurrentToken()->isKind(TokenKind::TokenPuncComma))
            {
                return ErrorTable::addError(getCurrentToken(), "Expected ) or , in argument list");
            }

            getNextToken();
        }
    }

    return std::make_shared<CallExpression>(callToken, callToken->getValue(), args);
}

std::shared_ptr<weasel::Expression> weasel::Parser::parseIdentifierExpression()
{
    auto identifier = getCurrentToken()->getValue();
    auto attr = SymbolTable::get(identifier);
    if (!attr)
    {
        return ErrorTable::addError(getCurrentToken(), "Variable not yet declared");
    }

    if (attr->getKind() == AttributeKind::SymbolFunction)
    {
        return parseFunctionCallExpression(attr);
    }

    // Check if Array Variable
    if (attr->isKind(AttributeKind::SymbolArray) || attr->isKind(AttributeKind::SymbolPointer))
    {
        if (expectToken(TokenKind::TokenDelimOpenSquareBracket))
        {
            getNextToken(); // eat identifier
            getNextToken(); // eat [
            auto indexExpr = parseExpression();

            return std::make_shared<ArrayExpression>(indexExpr->getToken(), identifier, indexExpr);
        }
    }

    return std::make_shared<VariableExpression>(getCurrentToken(), identifier);
}

std::shared_ptr<weasel::Expression> weasel::Parser::parseParenExpression()
{
    getNextToken(); // eat (
    auto expr = parseExpression();
    if (!expr)
    {
        return ErrorTable::addError(getCurrentToken(), "Expected expression inside after (..");
    }

    if (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseParen))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected )");
    }

    // Token ) will eated next time
    return expr;
}

std::shared_ptr<weasel::Expression> weasel::Parser::parseArrayExpression()
{
    auto expr = std::make_shared<ArrayLiteralExpression>();

    getNextToken(); // eat [
    while (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseSquareBracket))
    {
        expr->addItem(parseLiteralExpression());

        if (getNextToken()->isKind(TokenKind::TokenPuncComma))
        {
            getNextToken();
        }
    }

    return expr;
}

std::shared_ptr<weasel::Expression> weasel::Parser::parsePrimaryExpression()
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

    if (getCurrentToken()->isKind(TokenKind::TokenOperatorAnd))
    {
        if (getNextToken()->isKind(TokenKind::TokenIdentifier))
        {
            return std::make_shared<VariableExpression>(getCurrentToken(), getCurrentToken()->getValue(), true);
        }

        return ErrorTable::addError(getCurrentToken(), "Expected Variable Identifier for address of");
    }

    if (getCurrentToken()->isKind(TokenKind::TokenDelimOpenSquareBracket))
    {
        return parseArrayExpression();
    }

    return ErrorTable::addError(getCurrentToken(), "Expected expression");
}

std::shared_ptr<weasel::Expression> weasel::Parser::parseExpression()
{
    auto lhs = parsePrimaryExpression();
    if (!lhs)
    {
        return ErrorTable::addError(getCurrentToken(), "Expected LHS");
    }

    if (getNextToken()->isKind(TokenKind::TokenSpaceNewline))
    {
        return lhs;
    }

    return parseBinaryOperator(__defaultPrecOrder, lhs);
}

std::shared_ptr<weasel::Expression> weasel::Parser::parseBinaryOperator(unsigned precOrder, std::shared_ptr<Expression> lhs)
{
    while (true)
    {
        auto binOp = getCurrentToken();
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
        auto rhs = parsePrimaryExpression();
        if (!rhs)
        {
            return ErrorTable::addError(getCurrentToken(), "Expected RHS Expression 1");
        }

        getNextToken(); // eat 'rhs'
        rhs = parseBinaryOperator(prec.order, rhs);
        if (!rhs)
        {
            return ErrorTable::addError(getCurrentToken(), "Expected RHS Expression 2");
        }

        lhs = std::make_shared<BinaryOperatorExpression>(binOp, lhs, rhs);
    }
}

std::shared_ptr<weasel::Expression> weasel::Parser::parseReturnStatement()
{
    auto retToken = getCurrentToken();
    getNextToken(); // eat 'return'

    auto expr = parseExpression();
    if (!expr)
    {
        auto errToken = getCurrentToken();

        getNextTokenUntil(TokenKind::TokenSpaceNewline);
        return ErrorTable::addError(errToken, "Expected expression for return statement.");
    }

    return std::make_shared<ReturnExpression>(retToken, expr);
}

std::shared_ptr<weasel::Expression> weasel::Parser::parseCompoundStatement()
{
    auto stmt = std::make_shared<StatementExpression>();

    // Enter Statement Scope
    {
        SymbolTable::enterScope();
    }

    getNextToken(true); // eat '{'
    while (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseCurlyBracket))
    {
        auto expr = parseExpression();
        if (!expr)
        {
            return ErrorTable::addError(getCurrentToken(), "Expected statement");
        }

        stmt->addBody(expr);
    }
    getNextToken(true); // eat '}'

    // Exit Statement Scope
    {
        SymbolTable::exitScope();
    }

    return stmt;
}

// let 'identifier' 'datatype'  = 'expr'
// let 'identifier' 'datatype'
// let 'identifier'             = 'expr'
// let 'identifier' *'datatype'
// let 'identifier'             = &'expr'
// let 'identifier' [<size>]'datatype'
// let 'identifier'             = []
std::shared_ptr<weasel::Expression> weasel::Parser::parseDeclarationExpression()
{
    auto qualifier = getQualifier();
    auto qualToken = getCurrentToken();

    getNextToken(); // eat qualifier(let, final, const)
    if (!getCurrentToken()->isKind(TokenKind::TokenIdentifier))
    {
        auto errToken = getCurrentToken();

        getNextTokenUntil(TokenKind::TokenSpaceNewline);
        return ErrorTable::addError(errToken, "Expected an identifier");
    }

    auto identifier = getCurrentToken()->getValue();

    getNextToken(); // eat 'identifier' and get next token

    auto *type = parseDataType();
    if (getCurrentToken()->isKind(TokenKind::TokenSpaceNewline))
    {
        if (!type)
        {
            return ErrorTable::addError(getCurrentToken(), "Data Type Expected for default value declaration");
        }

        if (qualifier != Qualifier::QualVolatile)
        {
            return ErrorTable::addError(getCurrentToken(), "No Default Value for Non Volatile variable");
        }

        // Insert Symbol Table
        {
            AttributeKind attrKind;
            if (type->isArrayTy())
            {
                attrKind = AttributeKind::SymbolArray;
            }
            else if (type->isPointerTy())
            {
                attrKind = AttributeKind::SymbolPointer;
            }
            else
            {
                attrKind = AttributeKind::SymbolVariable;
            }

            auto attr = std::make_shared<Attribute>(identifier, AttributeScope::ScopeLocal, attrKind, type);
            SymbolTable::insert(identifier, attr);
        }

        // Create Variable with Default Value
        return std::make_shared<DeclarationExpression>(qualToken, identifier, qualifier, type);
    }

    // Equal
    if (!getCurrentToken()->isKind(TokenKind::TokenOperatorEqual))
    {
        auto errToken = getCurrentToken();

        getNextTokenUntil(TokenKind::TokenSpaceNewline);
        return ErrorTable::addError(errToken, "Expected equal sign");
    }

    // Get Value
    getNextToken(); // eat 'Equal Sign'
    if (getCurrentToken()->isKind(TokenKind::TokenSpaceNewline))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected RHS Value Expression but got 'New line'");
    }

    auto val = parseExpression();
    if (!val)
    {
        auto errToken = getCurrentToken();

        getNextTokenUntil(TokenKind::TokenSpaceNewline);
        return ErrorTable::addError(errToken, "Expected RHS Value Expression but got not valid expression");
    }

    // Insert Symbol Table
    {
        AttributeKind attrKind;
        if (type->isArrayTy())
        {
            attrKind = AttributeKind::SymbolArray;
        }
        else if (type->isPointerTy())
        {
            attrKind = AttributeKind::SymbolPointer;
        }
        else
        {
            attrKind = AttributeKind::SymbolVariable;
        }

        auto attr = std::make_shared<Attribute>(identifier, AttributeScope::ScopeLocal, attrKind, type);
        SymbolTable::insert(identifier, attr);
    }

    return std::make_shared<DeclarationExpression>(qualToken, identifier, qualifier, type, val);
}
