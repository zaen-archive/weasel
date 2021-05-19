#pragma once

#include "ast/ast.h"
#include "lex/lexer.h"

namespace underrated
{
    class Parser
    {
    private:
        Lexer *_lexer;

        Token *getCurrentToken() const { return _lexer->getCurrentToken(); }
        Token *getNextToken() const { return _lexer->getNextToken(); }

    private:
        ExprAST *parseVariableExpr();
        ExprAST *parseExpr();
        ExprAST *parseNumberExpr();
        // ExprAST *parseBinopExpr();
        // ExprAST *parseIdentifierExpr();

        // Function Part
        // ExprAST *parseStatement();
        // ExprAST *parseFunctionNameExpr();

    public:
        Parser(Lexer *lexer) : _lexer(lexer) {}

    public:
        ExprAST *parse();
    };
} // namespace underrated
