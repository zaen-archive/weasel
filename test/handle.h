#include <iostream>
#include "parser.h"

void handleDefinition()
{
    if (underrated::parseDefinition())
    {
        fprintf(stderr, "Parsed a function definition.\n");
    }
    else
    {
        underrated::getNextToken(); // skip token for error recovery

        fprintf(stderr, "Error parsing a function definition.\n");
    }
}

void handleExtern()
{
    if (underrated::parseExern())
    {
        fprintf(stderr, "Parsed an extern.\n");
    }
    else
    {
        fprintf(stderr, "Error parsing an extern.\n");

        underrated::getNextToken();
    }
}

void handleTopLevelExpr()
{
    auto handle = underrated::parseTopLevelExpr();
    if (handle)
    {
        fprintf(stderr, "Parsed a top-level expression.\n");
    }
    else
    {
        fprintf(stderr, "Errpr parsing a top-level expression.\n");

        underrated::getNextToken();
    }
}

void mainLoop()
{
    while (true)
    {
        fprintf(stderr, "Ready > ");
        auto token = underrated::getNextToken();

        std::cout << "Token : " << enumToInt(underrated::_currentToken_) << "\n";
        std::cout << "Token2 : " << enumToInt(token) << "\n";
        // auto token = underrated::_currentToken_;
        // if (token == underrated::TokenKind::TokenEOF)
        // {
        //     return;
        // }
        // else if (token == underrated::TokenKind::TokenSemicolon)
        // {
        //     underrated::getNextToken();
        // }
        // else if (token == underrated::TokenKind::TokenFun)
        // {
        //     handleDefinition();
        // }
        // else if (token == underrated::TokenKind::TokenExtern)
        // {
        //     handleExtern();
        // }
        // else
        // {
        //     handleTopLevelExpr();
        // }
    }
}
