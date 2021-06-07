#pragma once

#include "llvm/IR/IRBuilder.h"
#include "zero/ast/ast.h"
#include "zero/lex/lexer.h"

namespace zero
{
    class AnalysContext;

    class Parser
    {
    private:
        Lexer *_lexer;
        AnalysContext *_context;
        std::vector<Function *> _funcs;
        std::vector<Expression *> _global;

    public:
        void addFunction(Function *func) { _funcs.push_back(func); }

        Token *getCurrentToken() const { return _lexer->getCurrentToken(); }
        Token *getNextToken(bool skipSpace = false);
        Token *getNextTokenUntil(TokenKind kind);
        Qualifier getQualifier() const { return getCurrentToken()->getQualifier(); }
        bool expectToken(TokenKind kind) { return _lexer->expect(kind); }

        // Function
        Function *parseDeclareFunction();
        Function *parseFunction();
        std::vector<FunctionArgument *> parseFunctionArguments();
        FunctionArgument *parseFunctionArgument();

        // Statement
        StatementExpression *parseFunctionBody();
        Expression *parseStatement();
        Expression *parseCompoundStatement();
        Expression *parseReturnStatement();

        // Expression
        Expression *parseExpression();
        Expression *parsePrimaryExpression();
        Expression *parseDefinitionExpression();
        Expression *parseFunctionCallExpression(Attribute *attr);

        // Expression Literal
        Expression *parseLiteralExpression();
        Expression *parseIdentifierExpression();
        Expression *parseBinaryOperator(unsigned prec, zero::Expression *lhs);

        // Helper
        void ignoreNewline();

    public:
        Parser(AnalysContext *c, Lexer *lexer) : _lexer(lexer), _context(c) {}

        // getContext
        AnalysContext *getContext() const { return _context; }

        // getModule
        llvm::Module *getModule() const;

        // getBuilder
        llvm::IRBuilder<> *getBuilder() const;

        // setContext
        void setContext(AnalysContext *c) { _context = c; }

    public:
        // Parse Library in single file
        bool parse();

        // Codegen every function inside library
        void codegen();
    };
} // namespace zero
