#include <iostream>
#include "llvm/Support/TargetSelect.h"
#include "weasel/lex/lexer.h"
#include "weasel/analysis/context.h"
#include "weasel/parse/parser.h"

int main()
{
    // Initialize LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    auto *lexer = new weasel::Lexer();
    auto *context = new weasel::AnalysContext("UnderratedProject");
    auto *parser = new weasel::Parser(context, lexer);

    while (true)
    {
        std::cout << "Typing > ";

        parser->getNextToken();
        auto *expr = parser->parseExpression();
        llvm::outs() << expr << "\n";
    }
}
