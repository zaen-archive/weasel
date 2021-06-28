//
// Created by zaen on 27/06/21.
//
#include <llvm/IR/Function.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/Verifier.h>
#include <LLVMSPIRVLib.h>
#include "weasel/symbol/symbol.h"
#include "weasel/codegen/codegen.h"
#include "weasel/passes/passes.h"
#include "weasel/metadata/metadata.h"

void weasel::Codegen::compile()
{
    if (_funs.empty())
        return;

    auto cpu = "generic";
    auto targetTriple = _isParallel ? "spir64" : llvm::sys::getDefaultTargetTriple();
    auto features = "";
    auto dataLayout = llvm::DataLayout(llvm::StringRef());
    if (!_isParallel)
    {
        std::string err;
        auto *target = llvm::TargetRegistry::lookupTarget(targetTriple, err);
        if (!target)
        {
            _err = err;
            return;
        }

        auto targetOpts = llvm::TargetOptions();
        auto rm = llvm::Optional<llvm::Reloc::Model>();
        auto *targetMachine = target->createTargetMachine(targetTriple, cpu, features, targetOpts, rm);

        dataLayout = targetMachine->createDataLayout();
    }
    else
    {
        dataLayout = llvm::DataLayout("e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v16:16:16-v24:32:32-v32:32:32-v48:64:64-v64:64:64-v96:128:128-v128:128:128-v192:256:256-v256:256:256-v512:512:512-v1024:1024:1024");
    }

    getModule()->setTargetTriple(targetTriple);
    getModule()->setDataLayout(dataLayout);

    auto pass = std::make_unique<Passes>(getModule());
    auto ok = true;
    for (const auto &item : _funs)
    {
        auto identifier = item->getIdentifier();
        auto sym = SymbolTable::get(identifier);
        if (sym && sym->isKind(AttributeKind::SymbolFunction))
        {
            ok = false;
            break;
        }

        auto *fun = item->codegen(_context.get());
        if (!fun)
        {
            std::cerr << "Cannot codegen function " << identifier << "\n";
            ok = false;
            break;
        }

        if (llvm::verifyFunction(*fun, &llvm::errs()))
        {
            std::cerr << "Error when verifying function " << identifier << "\n";
            ok = false;
            break;
        }

        pass->run(*fun);
    }

    if (!ok)
    {
        std::cerr << "Error when constructing function\n";
        return;
    }

    auto meta = std::make_unique<Metadata>(getContext());
    if (_isParallel)
    {
        meta->initParallelModule(getModule());
    }
    else
    {
        meta->initModule(getModule());
    }

    if (llvm::verifyModule(*getModule()))
    {
        std::cerr << "Error when constructing module\n";
        return;
    }

    llvm::errs() << *getModule();

    std::cerr << "\n=> Error Information\n";
    ErrorTable::showErrors();

    // std::string errMsg;
    // std::ofstream file("main.main.spv");
    // std::stringbuf buf;
    // std::ostream OS(&buf);
    // bool val = llvm::regularizeLlvmForSpirv(getModule(), errMsg);
    // llvm::writeSpirv(getModule(), file, errMsg);
}
