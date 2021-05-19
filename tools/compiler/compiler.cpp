#include <iostream>
#include "parse/parser.h"

class Test
{
};

int main()
{
    // auto *token = new underrated::Token(underrated::TokenKind::TokenOpAssign);
    // auto *lhs = new underrated::VariableExprAST(token);
    // auto *rhs = new underrated::NumberExprAST(token);
    // auto *testing = new underrated::BinopExprAST(token, lhs, rhs);
    auto *lexer = new underrated::Lexer();
    auto *parser = new underrated::Parser(lexer);

    while (true)
    {
        std::cout << "Typing > ";
        auto *expr = parser->parse();

        if (expr)
        {
            expr->debug();
        }
        else
        {
            std::cout << "\n";
        }
    }
}
