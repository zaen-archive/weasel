#include "./parser.h"

namespace underrated
{
    std::unique_ptr<ExprAST> parseNumberExpr()
    {
        auto res = std::make_unique<NumberExprAST>(_numValue_);

        getNextToken();

        return move(res);
    }

    std::unique_ptr<ExprAST> parseParenExpr()
    {
        getNextToken(); // eat '('

        auto v = parseExpression();
        if (!v)
        {
            return nullptr;
        }

        if (_currentToken_ != TokenKind::TokenCloseParen)
        {
            return logError("Expected ')'");
        }

        getNextToken();

        return std::move(v);
    }

    std::unique_ptr<ExprAST> parseIdentifierExpr()
    {
        std::string name = _identifier_;

        getNextToken();

        if (_currentToken_ != TokenKind::TokenOpenParen)
        {
            return std::make_unique<VariableExprAST>(name);
        }

        // Call.
        getNextToken(); // eat (

        std::vector<std::unique_ptr<ExprAST>> args;
        if (_currentToken_ != TokenKind::TokenCloseParen)
        {
            while (true)
            {
                if (auto arg = parseExpression())
                {
                    args.push_back(std::move(arg));
                }
                else
                {
                    return nullptr;
                }

                if (_currentToken_ == TokenKind::TokenCloseParen)
                {
                    break;
                }

                if (_currentToken_ != TokenKind::TokenComma)
                {
                    return logError("Expected ')' or ',' in argument list");
                }

                getNextToken();
            }
        }
        // Eat )
        getNextToken();

        return std::make_unique<CallExprAST>(name, std::move(args));
    }

    std::unique_ptr<ExprAST> parsePrimary()
    {
        if (_currentToken_ == TokenKind::TokenIdentifier)
        {
            return parseIdentifierExpr();
        }

        if (_currentToken_ == TokenKind::TokenNumber)
        {
            return parseNumberExpr();
        }

        if (_currentToken_ == TokenKind::TokenOpenParen)
        {
            return parseParenExpr();
        }

        return logError("Unknown token when expecting an expression");
    }

    // a+b+(c+d)*e*f+g
    std::unique_ptr<ExprAST> parseExpression()
    {
        auto lhs = parsePrimary();
        if (!lhs)
        {
            return nullptr;
        }

        return parseBinopRhs(0, std::move(lhs));
    }

    std::unique_ptr<ExprAST> parseBinopRhs(int prec, std::unique_ptr<ExprAST> lhs)
    {
        while (true)
        {
            int tokprec = getTokenPrecedence();

            if (tokprec < prec)
            {
                return lhs;
            }

            TokenKind binop = _currentToken_;
            getNextToken(); // eat binop

            auto rhs = parsePrimary();
            if (!rhs)
            {
                return nullptr;
            }

            int nextPrec = getTokenPrecedence();
            if (tokprec < nextPrec)
            {
                rhs = parseBinopRhs(tokprec + 1, std::move(rhs));
                if (!rhs)
                {
                    return nullptr;
                }
            }
            lhs = std::make_unique<BinaryExprAST>(binop, std::move(lhs), std::move(rhs));
        }
    }

    // parsePrototype - Prototype Function
    std::unique_ptr<ProtorypeAST> parsePrototype()
    {
        if (_currentToken_ != TokenKind::TokenIdentifier)
        {
            return logErrorP("Expected function name in prototype");
        }

        std::string fnName = _identifier_;
        getNextToken();

        if (_currentToken_ != TokenKind::TokenOpenParen)
        {
            return logErrorP("Expected '(' in prototype");
        }

        // Read List Argument
        std::vector<std::string> args;
        while (getNextToken() == TokenKind::TokenIdentifier)
        {
            args.push_back(_identifier_);
        }

        if (_currentToken_ != TokenKind::TokenCloseParen)
        {
            return logErrorP("Expected ')' in prototype");
        }

        // Success parse parameters and function prototype
        getNextToken(); // eat ')'

        return std::make_unique<ProtorypeAST>(fnName, std::move(args));
    }

    // parseDefinition - Parsing function
    std::unique_ptr<FunctionAST> parseDefinition()
    {
        getNextToken(); // eat fun
        auto proto = parsePrototype();
        if (!proto)
        {
            return nullptr;
        }

        if (auto e = parseExpression())
        {
            return std::make_unique<FunctionAST>(std::move(proto), std::move(e));
        }

        return nullptr;
    }

    // parseExtern - parsing declare functions like 'sin' and 'cos'
    std::unique_ptr<ProtorypeAST> parseExern()
    {
        getNextToken(); // eat 'extern'

        return parsePrototype();
    }

    // parseTopLevelExpr - Parsing Top Level Expression
    std::unique_ptr<FunctionAST> parseTopLevelExpr()
    {
        if (auto e = parseExpression())
        {

            auto proto = std::make_unique<ProtorypeAST>(std::string(""), std::vector<std::string>());
            return std::make_unique<FunctionAST>(std::move(proto), std::move(e));
        }

        return nullptr;
    }
} // namespace Underrated
