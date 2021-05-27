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
        Qualifier getQualifier() const { return getCurrentToken()->getQualifierDefinition(); }

        // Function
        Function *parseDeclareFunction();
        Function *parseFunction();
        std::vector<FunctionArgument *> parseFunctionArguments();
        FunctionArgument *parseFunctionArgument();

        // Expression
        StatementExpression *parseBody();
        Expression *parseExpression();
        Expression *parseCompoundExpression();
        Expression *parseDefinitionExpression();

        // Expression Literal
        // TODO: Just suport 64 bit integer
        LiteralExpression *parseLiteralExpression();
        Expression *parseReturnExpression();

        Expression *parseVariableExpr();
        Expression *parseExpr();
        Expression *parseScalarExpr();

    public:
        Parser(AnalysContext *c, Lexer *lexer) : _lexer(lexer), _context(c) {}

        // getContext
        AnalysContext *getContext() const { return _context; }

        // GetLLVMContext
        llvm::LLVMContext *getLLVMContext() const;

        // getModule
        llvm::Module *getModule() const;

        // getBuilder
        llvm::IRBuilder<> *getBuilder() const;

        // setContext
        void setContext(AnalysContext *c) { _context = c; }

    public:
        void parse();
        void codegen();
    };
} // namespace underrated
