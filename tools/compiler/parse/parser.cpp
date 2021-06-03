#include <iostream>
#include "llvm/IR/Verifier.h"
#include "parse/parser.h"
#include "analysis/context.h"
#include "symbol/symbol.h"

// Codegen
void underrated::Parser::codegen()
{
    {
        if (SymbolTable::getInstance().getLookup().size() != 1)
        {
            std::cout << "\nSomething wrong with lookup table\n";
            for (auto item : SymbolTable::getInstance().getLookup())
            {
                std::cout << item << " ";
            }
            std::cout << "\n\n";

            return;
        }

        SymbolTable::getInstance().reset(); // We need to reset symbol table
    }

    auto ok = true;
    std::cout << "Functions : " << _funcs.size() << "\n";
    for (auto &item : _funcs)
    {
        auto identifier = item->getIdentifier();
        if (getModule()->getFunction(identifier))
        {
            std::cout << "Function " << identifier << " already declared\n";

            ok = false;
            break;
        }

        auto *fun = item->codegen(getContext());
        if (!fun)
        {
            std::cout << "\nSomthing happend to function symbol\n\n";
            return;
        }

        // Verify each function
        if (llvm::verifyFunction(*fun, &llvm::errs()))
        {
            std::cout << "\nError when verifying Function " << identifier << "\n";

            ok = false;
            break;
        }
    }

    if (!ok || llvm::verifyModule(*getContext()->getModule()))
    {
        std::cout << "Error when constructing functions\n";
    }

    llvm::errs() << *getContext()->getModule();

    std::cerr << "\n=> Error Information\n";
    ErrorTable::showErrors();
}

// parse
bool underrated::Parser::parse()
{
    // EOF FILE
    if (getCurrentToken()->isKind(TokenKind::TokenEOF))
    {
        return false;
    }

    // Extern Function
    if (getCurrentToken()->isKind(TokenKind::TokenKeyExtern))
    {
        getNextToken(); // eat 'extern'

        auto *func = parseDeclareFunction();
        if (func)
        {
            addFunction(func);
        }
        return true;
    }

    // Function
    if (getCurrentToken()->isKind(TokenKind::TokenKeyFun))
    {
        auto *func = parseFunction();
        if (func)
        {
            addFunction(func);
        }
        return true;
    }

    // Doing Global Variable
    // For latter implementation
    std::cout << "Parser -> " << enumToInt(getCurrentToken()->getTokenKind()) << " : " << getCurrentToken()->getValue() << "\n";
    return true;
}

// get Next Token Until
underrated::Token *underrated::Parser::getNextTokenUntil(underrated::TokenKind kind)
{
    if (getCurrentToken()->isKind(kind))
    {
        return getCurrentToken();
    }

    while (auto *token = getNextToken())
    {
        if (token->isKind(kind))
        {
            return token;
        }

        if (token->isKind(TokenKind::TokenEOF))
        {
            break;
        }

        delete token; // Delete Last Token
    }

    return nullptr;
}

// Get Next Token
underrated::Token *underrated::Parser::getNextToken(bool skipSpace)
{
    setLastToken(getCurrentToken());
    return _lexer->getNextToken(skipSpace);
}

// getModule
llvm::Module *underrated::Parser::getModule() const
{
    return _context->getModule();
}

// getBuilder
llvm::IRBuilder<> *underrated::Parser::getBuilder() const
{
    return _context->getBuilder();
}
