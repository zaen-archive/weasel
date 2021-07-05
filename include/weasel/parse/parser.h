#pragma once

#include "weasel/ast/ast.h"
#include "weasel/lex/lexer.h"

namespace weasel
{
    class Context;
    class Attribute;

    class Parser
    {
    private:
        std::unique_ptr<Lexer> _lexer;
        llvm::LLVMContext *_context;
        std::vector<std::shared_ptr<Function>> _funs;

        unsigned _parallelCount = 0;

    private:
        void addFunction(const std::shared_ptr<Function> &fun) { _funs.push_back(fun); }
        bool expectToken(TokenKind kind) { return _lexer->expect(kind); }

        Qualifier getQualifier() const { return getCurrentToken()->getQualifier(); }
        std::shared_ptr<Token> getCurrentToken() const { return _lexer->getCurrentToken(); }
        std::shared_ptr<Token> getNextToken(bool skipSpace = false);
        std::shared_ptr<Token> getNextTokenUntil(TokenKind kind);

        // Function
        std::shared_ptr<Function> parseDeclareFunction();
        std::shared_ptr<Function> parseFunction(ParallelType parallelType = ParallelType::None);
        std::shared_ptr<Function> parsePrallelFunction();

        // Statement
        std::shared_ptr<StatementExpression> parseFunctionBody();
        std::shared_ptr<Expression> parseStatement();
        std::shared_ptr<Expression> parseCompoundStatement();
        std::shared_ptr<Expression> parseReturnStatement();

        // Expression
        std::shared_ptr<Expression> parseExpression();
        std::shared_ptr<Expression> parsePrimaryExpression();
        std::shared_ptr<Expression> parseDeclarationExpression();
        std::shared_ptr<Expression> parseFunctionCallExpression(const std::shared_ptr<Attribute>& attr);
        std::shared_ptr<Expression> parseParenExpression();

        // Expression Literal
        std::shared_ptr<Expression> parseLiteralExpression();
        std::shared_ptr<Expression> parseIdentifierExpression();
        std::shared_ptr<Expression> parseBinaryOperator(unsigned prec, std::shared_ptr<weasel::Expression> lhs);
        std::shared_ptr<Expression> parseArrayExpression();

    public:
        Parser(llvm::LLVMContext* c, std::unique_ptr<Lexer> lexer) : _lexer(std::move(lexer)), _context(c) {}

        // getContext
        llvm::LLVMContext *getContext() const { return _context; }

        // Helper
        llvm::Type *parseDataType();

        void ignoreNewline();

        unsigned getParallelCount() const { return _parallelCount; }

        unsigned getFunctionCount() const { return _funs.size(); }

        bool isParallelExist() const { return _parallelCount > 0; }

        std::vector<std::shared_ptr<Function>> getFunctions() const { return _funs; };

        std::vector<std::shared_ptr<Function>> getParallelFunctions();

    public:
        void parse();
    };

} // namespace weasel
