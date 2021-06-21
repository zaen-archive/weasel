#include <fstream>
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "weasel/parse/parser.h"
#include "weasel/analysis/context.h"
#include "weasel/ast/ast.h"
#include "weasel/symbol/symbol.h"
#include "weasel/basic/filemanager.h"

#define __writeout

void emitObj(weasel::AnalysContext *context)
{
    // Compile to Object Code
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    context->getModule()->setTargetTriple(targetTriple);

    // Registry Error
    std::string error;
    auto *target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

    if (!target)
    {
        llvm::errs() << error;
        exit(1);
    }

    auto cpu = "generic";
    auto features = "";

    llvm::TargetOptions targetOpts;
    auto rm = llvm::Optional<llvm::Reloc::Model>();
    auto targetMachine = target->createTargetMachine(targetTriple, cpu, features, targetOpts, rm);

    context->getModule()->setDataLayout(targetMachine->createDataLayout());

    auto objFilename = "runtime-rt/main.o";

    std::error_code errorCode;
    llvm::raw_fd_ostream dest(objFilename, errorCode, llvm::sys::fs::OF_None);
    if (errorCode)
    {
        llvm::errs() << "Could not open file : " << errorCode.message();
        exit(1);
    }

    llvm::legacy::PassManager pass;
    auto fileType = llvm::CodeGenFileType::CGFT_ObjectFile;
    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType))
    {
        llvm::errs() << "The Target Machine can't emit a file of this type";
        exit(1);
    }

    pass.run(*context->getModule());
    dest.flush();

    llvm::outs() << "Wrote " << objFilename << "\n";
}

// void emitIr(weasel::AnalysContext *context)
// {
//     auto irFilename = "runtime-rt/main.ll";
// }

// Parser => Recursive descent parser
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

    auto *lexer = new weasel::Lexer(fileManager);
    auto *context = new weasel::AnalysContext("UnderratedModule");
    auto *parser = new weasel::Parser(context, lexer);

    /// Compiler ///
    do
    {
        parser->getNextToken(true);
    } while (parser->parse());

    parser->codegen(); // change ast to llvm IR

#ifdef __writeout

    // Initialize LLVM TO BULK
    llvm::InitializeAllTargetInfos();
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();

    emitObj(context);
#endif
}
