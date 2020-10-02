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
#include <memory>
#include <iostream>

llvm::Function *createFun(llvm::Module *m, llvm::LLVMContext &c) {
    // Default Type
    auto *int32Ty = llvm::Type::getInt32Ty(c);

    // Functions
    llvm::FunctionType *funTy = llvm::FunctionType::get(int32Ty, false);
    llvm::Function *fun = llvm::Function::Create(funTy, llvm::Function::LinkageTypes::ExternalLinkage, "test", m);

    // Init Block
    auto *entry = llvm::BasicBlock::Create(c, "", fun);

    // Align Value
    auto *retVal = new llvm::AllocaInst(int32Ty, 0, "", entry);
    new llvm::StoreInst(llvm::ConstantInt::get(int32Ty, 0), retVal, entry);

    // return a
    llvm::ReturnInst::Create(c, new llvm::LoadInst(int32Ty, retVal, "", entry), entry);

    return fun;
}

int main(int argc, char **argv) {
    // Argument
    // int n = argc > 1 ? atol(argv[1]) : 24;

    // Initialize LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    llvm::LLVMContext context;

    // Create Module
    // llvm/IR/Module
    std::unique_ptr<llvm::Module> owner(new llvm::Module("myModule", context));
    llvm::Module *module = owner.get();

    // Create Functions
    llvm::Function *func = createFun(module, context);

    std::cout << "Verifying .....\n";
    if (llvm::verifyModule(*module)) {
        std::cout << "Error Constructing Function!\n";
        return 1;
    }
    llvm::errs() << *module;

    return 0;
}
