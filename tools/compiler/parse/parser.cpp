#include <iostream>
#include "parse/parser.h"
#include "analysis/context.h"

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

    // Extern Function
    if (getCurrentToken()->isKind(TokenKind::TokenKeyExtern))
    {
        getNextToken(); // eat 'extern'

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

// getLLVMContext
llvm::LLVMContext *underrated::Parser::getLLVMContext() const
{
    return _context->getContext();
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
