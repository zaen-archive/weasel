#include <fstream>
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "zero/parse/parser.h"
#include "zero/analysis/context.h"
#include "zero/ast/ast.h"
#include "zero/symbol/symbol.h"
#include "zero/basic/filemanager.h"

// Parser => Recursive descent parser
int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        std::cout << "Need Argument\n";
        return 0;
    }

    auto *filePath = argv[1];
    auto *fileManager = new zero::FileManager(filePath);

    if (!fileManager->isValid())
    {
        std::cout << filePath << " Not exist\n";
        return 0;
    }

    // Initialize LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    auto *lexer = new zero::Lexer(fileManager);
    auto *context = new zero::AnalysContext("UnderratedModule");
    auto *parser = new zero::Parser(context, lexer);

    /// Compiler ///
    do
    {
        parser->getNextToken(true);
    } while (parser->parse());

    parser->codegen(); // change ast to llvm IR
}
