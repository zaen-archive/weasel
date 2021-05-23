#pragma once

#include "ast/ast.h"
#include "lex/lexer.h"
#include "analysis/context.h"

namespace underrated
{
    class Parser
    {
    private:
        Lexer *_lexer;
        AnalysContext *_context;
        std::vector<Func *> _funcs;

        void addFunction(Func *func) { _funcs.push_back(func); }
        Token *getCurrentToken() const { return _lexer->getCurrentToken(); }
        Token *getNextToken() const { return _lexer->getNextToken(); }

    private:
        // Function
        Func *parseDeclareFunction();
        Func *parseFunction();
        std::vector<FuncArg *> parseFunctionArguments();
        FuncArg *parseFunctionArgument();

        // Expression
        Expression *parseVariableExpr();
        Expression *parseExpr();
        Expression *parseScalarExpr();
        // ExprAST *parseBinopExpr();
        // ExprAST *parseIdentifierExpr();

        // Function Part
        // ExprAST *parseStatement();
        // ExprAST *parseFunctionNameExpr();

    public:
        Parser(AnalysContext *c, Lexer *lexer) : _lexer(lexer), _context(c) {}
        AnalysContext *getContext() const { return _context; }
        llvm::LLVMContext *getLLVMContext() const { return _context->getContext(); }
        llvm::Module *getModule() const { return _context->getModule(); }
        llvm::IRBuilder<> *getBuilder() const { return _context->getBuilder(); }

        void setContext(AnalysContext *c) { _context = c; }

    public:
        void parse();
        void codegen();
    };
} // namespace underrated
