//
// Created by zaen on 27/06/21.
//
#include <fstream>
#include <llvm/IR/Function.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <LLVMSPIRVLib.h>
#include "weasel/symbol/symbol.h"
#include "weasel/codegen/codegen.h"
#include "weasel/passes/passes.h"
#include "weasel/metadata/metadata.h"

weasel::Codegen::Codegen(std::unique_ptr<Context> context, std::vector<std::shared_ptr<Function>> funs)
{
    _context = std::move(context);
    _funs = std::move(funs);
    _isParallel = _context->isParallel();
}

bool weasel::Codegen::compile(const std::string &spirvIR)
{
    if (_funs.empty())
        return _isParallel;

    auto isHostCL = !_isParallel && !spirvIR.empty();
    if (isHostCL)
    {
        auto *builder = _context->getBuilder();
        auto *value = builder->getInt32(spirvIR.size());
        auto linkage = llvm::GlobalVariable::LinkageTypes::PrivateLinkage;
        auto *spirvSource = builder->CreateGlobalString(spirvIR, WEASEL_KERNEL_SOURCE_NAME + std::string(".str"), 0, getModule());
        auto idxList = std::vector<llvm::Value *>{
            builder->getInt64(0),
            builder->getInt64(0)};
        auto *gep = llvm::ConstantExpr::getGetElementPtr(spirvSource->getType()->getElementType(), spirvSource, idxList, true);

        new llvm::GlobalVariable(*getModule(), builder->getInt8PtrTy(), true, linkage, gep, WEASEL_KERNEL_SOURCE_NAME);
        new llvm::GlobalVariable(*getModule(), builder->getInt32Ty(), true, linkage, value, WEASEL_KERNEL_SOURCE_SIZE_NAME);
    }

    _context->setHostCL(isHostCL);

    auto pass = std::make_unique<Passes>(getModule());
    for (const auto &item : _funs)
    {
        auto identifier = item->getIdentifier();
        auto sym = SymbolTable::get(identifier);
        if (sym && sym->isKind(AttributeKind::SymbolFunction))
        {
            _err = "Function conflict : " + identifier + "\n";
            return false;
        }

        auto *fun = item->codegen(_context.get());
        if (!fun)
        {
            _err = "Cannot codegen function " + identifier + "\n";
            return false;
        }

        if (llvm::verifyFunction(*fun, &llvm::errs()))
        {
            _err = "Error when verifying function " + identifier + "\n";
            return false;
        }

        pass->run(*fun);
    }

    auto cpu = "generic";
    auto targetTriple = _isParallel ? "spir64" : llvm::sys::getDefaultTargetTriple();
    auto features = "";
    auto dataLayout = llvm::DataLayout(llvm::StringRef());
    if (!_isParallel)
    {
        auto *target = llvm::TargetRegistry::lookupTarget(targetTriple, _err);
        if (!target)
            return false;

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
        _err = "Error when constructing module\n";
        return false;
    }

    if (!ErrorTable::getErrors().empty())
    {
        std::cerr << "\n=> Error Information\n";
        ErrorTable::showErrors();
    }

    return true;
}

std::string weasel::Codegen::createSpirv()
{
    std::ostringstream ir;
    if (!_isParallel)
    {
        return "";
    }

    if (_funs.empty())
    {
        return "";
    }

    if (!llvm::regularizeLlvmForSpirv(getModule(), _err))
    {
        return "";
    }

    if (!llvm::writeSpirv(getModule(), ir, _err))
    {
        return "";
    }

    ir.flush();

    return ir.str();
}

void weasel::Codegen::createObject() const
{
    auto filePath = "runtime-rt/main.o";

    std::string err;
    std::error_code errCode;
    llvm::raw_fd_ostream dest(filePath, errCode, llvm::sys::fs::OF_None);
    if (errCode)
    {
        llvm::errs() << "Could not open file : " << errCode.message() << "\n";
        return;
    }

    auto pass = llvm::legacy::PassManager();
    auto fileType = llvm::CodeGenFileType::CGFT_ObjectFile;
    auto targetTriple = getModule()->getTargetTriple();
    auto *target = llvm::TargetRegistry::lookupTarget(targetTriple, err);
    if (!target)
    {
        llvm::errs() << err;
        exit(1);
    }

    auto cpu = "generic";
    auto features = "";
    auto rm = llvm::Optional<llvm::Reloc::Model>();
    auto targetOpts = llvm::TargetOptions();
    auto targetMachine = target->createTargetMachine(targetTriple, cpu, features, targetOpts, rm);

    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType))
    {
        llvm::errs() << "The Target Machine can't emit a file of this type";
        exit(1);
    }

    pass.run(*getModule());
    dest.flush();
}
