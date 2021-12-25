#include <sstream>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/IR/LegacyPassManager.h>
#include "weasel/parse/parser.h"
#include "weasel/ir/context.h"
#include "weasel/ast/ast.h"
#include "weasel/symbol/symbol.h"
#include "weasel/basic/filemanager.h"
#include "weasel/codegen/codegen.h"

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        std::cerr << "Not Input files\n";
        return 1;
    }

    auto *filePath = argv[1];
    auto *outputPath = argv[2];
    auto *fileManager = new weasel::FileManager(filePath);
    if (!fileManager->isValid())
    {
        std::cout << filePath << " Not exist\n";
        return 0;
    }

    // Initialize LLVM TO BULK
    // llvm::InitializeAllTargetInfos();
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();

    auto *llvmContext = new llvm::LLVMContext();
    auto lexer = std::make_unique<weasel::Lexer>(fileManager);
    auto parser = new weasel::Parser(llvmContext, std::move(lexer));

    // Parse into AST
    parser->parse();

    // Prepare for codegen
    auto context = std::make_unique<weasel::Context>(llvmContext, "CodeModule");
    auto codegen = std::make_unique<weasel::Codegen>(std::move(context), parser->getFunctions());

    weasel::SymbolTable::reset();
    if (!codegen->compile())
    {
        std::cerr << codegen->getError() << "\n";
        exit(1);
    }

    // Compile to Object
    codegen->createObject(outputPath);
}
