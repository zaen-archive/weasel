#include <iostream>
#include <unistd.h>
#include "llvm/IR/Verifier.h"
#include "weasel/parse/parser.h"
#include "weasel/ir/context.h"
#include "weasel/symbol/symbol.h"

// parse
void weasel::Parser::parse()
{
    while (!getNextToken(true)->isKind(TokenKind::TokenEOF))
    {

        // Parallel Function
        if (getCurrentToken()->isKind(TokenKind::TokenKeyParallel))
        {
            auto funParallel = parsePrallelFunction();
            if (funParallel)
            {
                _parallelCount++;

                addFunction(funParallel);
            }
            continue;
        }

        // Extern Function
        if (getCurrentToken()->isKind(TokenKind::TokenKeyExtern))
        {
            getNextToken(); // eat 'extern'
            auto func = parseFunction();
            if (func)
            {
                addFunction(func);
            }
            continue;
        }

        // Function
        if (getCurrentToken()->isKind(TokenKind::TokenKeyFun))
        {
            auto fun = parseFunction();
            if (fun)
            {
                addFunction(fun);
            }
            continue;
        }

        // TODO: Doing Global Variable
        // For latter implementation
        auto token = getCurrentToken();
        std::cout << "Parser -> " << token->getLocation().row << "/" << token->getLocation().col << "<>" << token->getTokenKindToInt() << ":" << token->getValue() << "\n";
    }
}

// get Next Token Until
std::shared_ptr<weasel::Token> weasel::Parser::getNextTokenUntil(weasel::TokenKind kind)
{
    if (getCurrentToken()->isKind(kind))
    {
        return getCurrentToken();
    }

    while (auto token = getNextToken())
    {
        if (token->isKind(kind))
        {
            return token;
        }

        if (token->isKind(TokenKind::TokenEOF))
        {
            break;
        }
    }

    return nullptr;
}

// Get Next Token
std::shared_ptr<weasel::Token> weasel::Parser::getNextToken(bool skipSpace)
{
    return _lexer->getNextToken(skipSpace);
}
