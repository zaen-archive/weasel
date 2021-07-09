#include <iostream>
#include <memory>
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "weasel/parse/parser.h"
#include "weasel/ast/ast.h"
#include "weasel/ir/context.h"

int main()
{
    // Initialize LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    auto *llvmContext = new llvm::LLVMContext();
    auto *module = new llvm::Module("unittest-llvm", *llvmContext);
    auto *builder = new llvm::IRBuilder<>(*llvmContext);
    auto *funTy = llvm::FunctionType::get(builder->getInt32Ty(), false);
    auto *fun = llvm::Function::Create(funTy, llvm::Function::ExternalLinkage, "testing", *module);
    auto *block = llvm::BasicBlock::Create(*llvmContext, "entry", fun);

    builder->SetInsertPoint(block);

    auto *str = builder->CreateGlobalStringPtr("Hello World", "msg");
    std::vector<llvm::Value *> callParams;
    callParams.push_back(str);

    // Init printf
    auto *pty = llvm::PointerType::get(llvm::IntegerType::get(*llvmContext, 8), 0);
    auto *fty = llvm::FunctionType::get(llvm::IntegerType::get(*llvmContext, 32), true);
    auto *fpf = llvm::Function::Create(fty, llvm::GlobalValue::ExternalLinkage, "puts", *module);

    // Verify Function
    llvm::verifyFunction(*fun);

    // Create JIT
    std::string errMsg;
    llvm::ExecutionEngine *ee = llvm::EngineBuilder(std::move(std::unique_ptr<llvm::Module>(module))).setErrorStr(&errMsg).create();
    if (!ee)
    {
        std::cout << "Failed to construct ExecutionEngine: " << errMsg << '\n';
        return 1;
    }

    // Verifying Module
    std::cout << "Verifying ...\n";
    if (llvm::verifyModule(*module))
    {
        std::cout << "Error Constructing Function!\n";
        return 1;
    }

    // Show Module
    llvm::errs() << *module;

    std::cout << "Run Function\n";
    std::vector<llvm::GenericValue> args(0);
    llvm::GenericValue gv = ee->runFunction(fun, args);

    llvm::outs() << "Result : " << gv.IntVal << "\n";
}