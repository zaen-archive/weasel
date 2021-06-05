#include <iostream>
#include "llvm/Support/TargetSelect.h"
#include "lex/lexer.h"
#include "analysis/context.h"
#include "parse/parser.h"

int main()
{
    // Initialize LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    auto *lexer = new zero::Lexer();
    auto *context = new zero::AnalysContext("UnderratedProject");
    auto *parser = new zero::Parser(context, lexer);

    while (true)
    {
        std::cout << "Typing > ";

        parser->getNextToken();
        auto *expr = parser->parseExpression();
        llvm::outs() << expr << "\n";
    }
}
