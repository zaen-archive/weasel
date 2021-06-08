#include <iostream>
#include "llvm/IR/Verifier.h"
#include "zero/parse/parser.h"
#include "zero/analysis/context.h"
#include "zero/symbol/symbol.h"

// Codegen
void zero::Parser::codegen()
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
bool zero::Parser::parse()
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

    // TODO: Doing Global Variable
    // For latter implementation
    auto *token = getCurrentToken();
    std::cout << "Parser -> " << token->getLocation().row << "/" << token->getLocation().col << "<>" << token->getTokenKindToInt() << ":" << token->getValue() << "\n";
    return true;
}

// get Next Token Until
zero::Token *zero::Parser::getNextTokenUntil(zero::TokenKind kind)
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
    }

    return nullptr;
}

// Get Next Token
zero::Token *zero::Parser::getNextToken(bool skipSpace)
{
    return _lexer->getNextToken(skipSpace);
}

// getModule
llvm::Module *zero::Parser::getModule() const
{
    return _context->getModule();
}

// getBuilder
llvm::IRBuilder<> *zero::Parser::getBuilder() const
{
    return _context->getBuilder();
}
