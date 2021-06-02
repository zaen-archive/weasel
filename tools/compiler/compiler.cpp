#include <fstream>
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "parse/parser.h"
#include "analysis/context.h"
#include "ast/ast.h"
#include "symbol/symbol.h"

// Parser => Recursive descent parser
int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        std::cout << "Need Argument\n";
        return 0;
    }

    char *filePath = argv[1];
    std::ifstream stream(filePath);
    if (!stream.is_open())
    {
        std::cout << filePath << " Not exist\n";
    }

    // Initialize LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    auto *lexer = new underrated::Lexer(&stream);
    auto *context = new underrated::AnalysContext("UnderratedModule");
    auto *parser = new underrated::Parser(context, lexer);

    /// Compiler ///
    while (true)
    {
        parser->getNextToken(true);
        if (!parser->parse())
        {
            break;
        }
    }

    parser->codegen(); // change ast to llvm IR
    (new underrated::DebugExpression())->codegen(context);

    underrated::ErrorTable::showErrors();
}
