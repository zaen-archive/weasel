#include <iostream>
#include "parse/parser.h"

// Codegen
void underrated::Parser::codegen()
{
    for (auto &item : _funcs)
    {
        auto *func = getModule()->getFunction(item->getIdentifier());
        if (!func)
        {
            item->codegen(getContext());
        }
    }
}

// parse
void underrated::Parser::parse()
{
    getNextToken(); // start parsing

    // Debugging
    if (getCurrentToken()->isDebug())
    {
        getNextToken(); // Eat 'debug' away
        codegen();      // change ast to llvm IR
        (new DebugExpression())->codegen(getContext());
        return;
    }

    // Declare Function
    if (getCurrentToken()->isKind(TokenKind::TokenKeyExport))
    {
        getNextToken(); // eat 'export'

        auto *func = parseDeclareFunction();
        if (func)
        {
            addFunction(func);
        }
        return;
    }

    // Function
    if (getCurrentToken()->isKind(TokenKind::TokenKeyFun))
    {
        auto *func = parseFunction();
        if (func)
        {
            addFunction(func);
        }
        return;
    }

    std::cout << enumToInt(getCurrentToken()->getTokenKind()) << ":" << getCurrentToken()->getValue() << "\n";
}

// underrated::Expr *underrated::Parser::parse()
// {
//     auto *token = getNextToken();

//     // Function
//     if (token->isFunctionDeclare())
//     {
//         return parseFunction();
//     }

//     if (token->getTokenKind() == TokenKind::TokenDebug)
//     {
//         getNextToken(); // eat 'debug' token

//         return new DebugExpr();
//     }

//     if (token->getTokenKind() == TokenKind::TokenKeyLet)
//     {
//         return this->parseVariableExpr();
//     }

//     return logError("Expecting new code here!");
// }
