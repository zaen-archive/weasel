#pragma once

#include "llvm/IR/IRBuilder.h"
#include "zero/ast/ast.h"
#include "zero/lex/lexer.h"

namespace weasel
{
    class AnalysContext;
    class Attribute;

    class Parser
    {
    private:
        Lexer *_lexer;
        AnalysContext *_context;
        std::vector<std::shared_ptr<Function>> _funcs;
        std::vector<std::shared_ptr<Expression>> _global;

    public:
        void addFunction(std::shared_ptr<Function> func) { _funcs.push_back(func); }
        bool expectToken(TokenKind kind) { return _lexer->expect(kind); }

        Qualifier getQualifier() const { return getCurrentToken()->getQualifier(); }
        std::shared_ptr<Token> getCurrentToken() const { return _lexer->getCurrentToken(); }
        std::shared_ptr<Token> getNextToken(bool skipSpace = false);
        std::shared_ptr<Token> getNextTokenUntil(TokenKind kind);

        // Function
        std::shared_ptr<Function> parseDeclareFunction();
        std::shared_ptr<Function> parseFunction();
        // std::vector<std::shared_ptr<FunctionArgument>> parseFunctionArguments();
        // std::shared_ptr<FunctionArgument> parseFunctionArgument();

        // Statement
        std::shared_ptr<StatementExpression> parseFunctionBody();
        std::shared_ptr<Expression> parseStatement();
        std::shared_ptr<Expression> parseCompoundStatement();
        std::shared_ptr<Expression> parseReturnStatement();

        // Expression
        std::shared_ptr<Expression> parseExpression();
        std::shared_ptr<Expression> parsePrimaryExpression();
        std::shared_ptr<Expression> parseDeclarationExpression();
        std::shared_ptr<Expression> parseFunctionCallExpression(std::shared_ptr<Attribute> attr);
        std::shared_ptr<Expression> parseParenExpression();

        // Expression Literal
        std::shared_ptr<Expression> parseLiteralExpression();
        std::shared_ptr<Expression> parseIdentifierExpression();
        std::shared_ptr<Expression> parseBinaryOperator(unsigned prec, std::shared_ptr<weasel::Expression> lhs);

        // Helper
        llvm::Type *parseDataType();
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
} // namespace weasel
