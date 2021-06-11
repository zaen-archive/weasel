#include <fstream>
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "zero/parse/parser.h"
#include "zero/analysis/context.h"
#include "zero/ast/ast.h"
#include "zero/symbol/symbol.h"
#include "zero/basic/filemanager.h"

// Parser => Recursive descent parser
int main(int argc, char *argv[])
{
    char *filePath;
    if (argc <= 1)
    {
        filePath = (char *)"G:/Compiler/underrated_project/runtime-rt/main.ze";
    }
    else
    {
        filePath = argv[1];
    }

    auto *fileManager = new zero::FileManager(filePath);
    if (!fileManager->isValid())
    {
        std::cout << filePath << " Not exist\n";
        return 0;
    }

    auto *lexer = new zero::Lexer(fileManager);
    auto *context = new zero::AnalysContext("UnderratedModule");
    auto *parser = new zero::Parser(context, lexer);

    /// Compiler ///
    do
    {
        parser->getNextToken(true);
    } while (parser->parse());

    parser->codegen(); // change ast to llvm IR

    // // Initialize LLVM TO BULK
    // // llvm::InitializeAllTargetInfos();
    // // llvm::InitializeAllTargets();
    // // llvm::InitializeAllTargetMCs();
    // // llvm::InitializeAllAsmParsers();
    // // llvm::InitializeAllAsmPrinters();
    // // Initialize just native Machine
    // llvm::InitializeAllTargetInfos();
    // // llvm::InitializeAllTargetMCs();
    // llvm::InitializeNativeTarget();
    // llvm::InitializeNativeTargetAsmParser();
    // llvm::InitializeNativeTargetAsmPrinter();

    // // Compile to Object Code
    // auto targetTriple = llvm::sys::getDefaultTargetTriple();
    // context->getModule()->setTargetTriple(targetTriple);

    // // Registry Error
    // std::string error;
    // auto *target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

    // if (!target)
    // {
    //     llvm::errs() << error;
    //     return 1;
    // }

    // auto cpu = "generic";
    // auto features = "";

    // llvm::TargetOptions targetOpts;
    // auto rm = llvm::Optional<llvm::Reloc::Model>();
    // auto targetMachine = target->createTargetMachine(targetTriple, cpu, features, targetOpts, rm);

    // context->getModule()->setDataLayout(targetMachine->createDataLayout());

    // auto filename = "test/output.o";
    // std::error_code errorCode;
    // llvm::raw_fd_ostream dest(filename, errorCode, llvm::sys::fs::OF_None);
    // if (errorCode)
    // {
    //     llvm::errs() << "Could not open file : " << errorCode.message();
    //     return 1;
    // }

    // llvm::legacy::PassManager pass;
    // auto fileType = llvm::CGFT_ObjectFile;
    // if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType))
    // {
    //     llvm::errs() << "The Target Machine can't emit a file of this type";
    //     return 1;
    // }

    // pass.run(*context->getModule());
    // dest.flush();

    // llvm::outs() << "Wrote " << filename << "\n";
}
