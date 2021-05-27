#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "parse/parser.h"
#include "analysis/context.h"

// Parser => Recursive descent parser
int main()
{
    // Initialize LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    auto *lexer = new underrated::Lexer();
    auto *context = new underrated::AnalysContext("UnderratedModule");
    auto *parser = new underrated::Parser(context, lexer);

    /// Compiler ///
    while (true)
    {
        llvm::errs() << "Typing > ";

        parser->parse();
    }
}
