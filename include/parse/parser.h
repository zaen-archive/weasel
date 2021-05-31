#pragma once

#include "llvm/IR/IRBuilder.h"
#include "ast/ast.h"
#include "lex/lexer.h"

namespace underrated
{
    class AnalysContext;

    class Parser
    {
    private:
        Lexer *_lexer;
        AnalysContext *_context;
        std::vector<Function *> _funcs;

    public:
        void addFunction(Function *func) { _funcs.push_back(func); }
        Token *getCurrentToken() const { return _lexer->getCurrentToken(); }
        Token *getNextToken(bool skipSpace = false) const { return _lexer->getNextToken(skipSpace); }
        Qualifier getQualifier() const { return getCurrentToken()->getQualifier(); }

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
        // Expression Literal
        // TODO: Just suport 64 bit integer
        Expression *parseLiteralExpression();
        Expression *parseVariableExpression();

        Expression *parseBinaryOperator(int prec, underrated::Expression *lhs);

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
        void parse();

        // Codegen every function inside library
        void codegen();
    };
} // namespace underrated
