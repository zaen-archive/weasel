#include <sstream>
#include "llvm/Support/TargetSelect.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/IR/LegacyPassManager.h"
#include "weasel/parse/parser.h"
#include "weasel/ir/context.h"
#include "weasel/ast/ast.h"
#include "weasel/symbol/symbol.h"
#include "weasel/basic/filemanager.h"
#include "weasel/codegen/codegen.h"

// void emitObj(weasel::Context *context)
// {
//     // Compile to Object Code
//     auto targetTriple = llvm::sys::getDefaultTargetTriple();
//     context->getModule()->setTargetTriple(targetTriple);
//
//     // Registry Error
//     std::string error;
//     auto *target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
//     if (!target)
//     {
//         llvm::errs() << error;
//         exit(1);
//     }
//
//     auto cpu = "generic";
//     auto features = "";
//
//     llvm::TargetOptions targetOpts;
//     auto rm = llvm::Optional<llvm::Reloc::Model>();
//     auto targetMachine = target->createTargetMachine(targetTriple, cpu, features, targetOpts, rm);
//
//     context->getModule()->setDataLayout(targetMachine->createDataLayout());
//
//     // Create Object Output and Create Binary Output
//     auto objFilename = "runtime-rt/main.o";
//     std::error_code errorCode;
//     llvm::raw_fd_ostream dest(objFilename, errorCode, llvm::sys::fs::OF_None);
//     if (errorCode)
//     {
//         llvm::errs() << "Could not open file : " << errorCode.message();
//         exit(1);
//     }
//
//     auto pass = llvm::legacy::PassManager();
//     auto fileType = llvm::CodeGenFileType::CGFT_ObjectFile;
//     if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType))
//     {
//         llvm::errs() << "The Target Machine can't emit a file of this type";
//         exit(1);
//     }
//
//     pass.run(*context->getModule());
//     dest.flush();
//
//     llvm::outs() << "Wrote " << objFilename << "\n";
// }

int main(int argc, char *argv[])
{
    char *filePath;
    if (argc <= 1)
    {
        std::cerr << "Not Input files\n";
        return 1;
    }
    else
    {
        filePath = argv[1];
    }

    auto *fileManager = new weasel::FileManager(filePath);
    if (!fileManager->isValid())
    {
        std::cout << filePath << " Not exist\n";
        return 0;
    }

    // Initialize LLVM TO BULK
    llvm::InitializeAllTargetInfos();
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();

    auto *llvmContext = new llvm::LLVMContext();
    auto lexer = std::make_unique<weasel::Lexer>(fileManager);
    auto parser = new weasel::Parser(llvmContext, std::move(lexer));

    // Parse into AST
    parser->parse();

    // Reset Symbol Table
    weasel::SymbolTable::reset();

    // Prepare for codegen
    auto spirContext = std::make_unique<weasel::Context>(llvmContext, "SpirVModule");
    auto context = std::make_unique<weasel::Context>(llvmContext, "CodeModule");
    auto spirCodegen = std::make_unique<weasel::Codegen>(std::move(spirContext), parser->getParallelFunctions(), true);
    auto codegen = std::make_unique<weasel::Codegen>(std::move(context), parser->getFunctions());

    // Compile
    spirCodegen->compile();
    codegen->compile();

    // TODO: Do something after magic happend
}
