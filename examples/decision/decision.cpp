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


// Simple Operation
/*
func something() {
    let a = true
    if (a) {
        a = false
    }
    return d
}
*/
llvm::Function *createFun(llvm::Module *m, llvm::LLVMContext &c) {
    // Default Type
    auto *int32Ty = llvm::Type::getInt32Ty(c);

    // Functions
    llvm::FunctionType *funTy = llvm::FunctionType::get(int32Ty, false);
    llvm::Function *fun = llvm::Function::Create(funTy, llvm::Function::LinkageTypes::ExternalLinkage, "test", m);

    // Init Block
    auto *entry = llvm::BasicBlock::Create(c, "entry", fun);
    auto *ifBlock = llvm::BasicBlock::Create(c, "ifblock", fun);
    auto *endBlock = llvm::BasicBlock::Create(c, "endblock", fun);

    // Align Value
    auto *alignVal = new llvm::AllocaInst(int32Ty, 0, "", entry);
    new llvm::StoreInst(llvm::ConstantInt::get(int32Ty, 0), alignVal, entry);

    // Entry Block
    // int a = 10
    auto *a = new llvm::AllocaInst(int32Ty, 0, "", entry);
    new llvm::StoreInst(llvm::ConstantInt::get(int32Ty, 10), a, entry);

    // final b = 20
    llvm::Value *b = llvm::ConstantInt::get(int32Ty, 20);

    // if a > b
    auto *cond = llvm::ICmpInst::Create(llvm::Instruction::OtherOps::ICmp,
        llvm::CmpInst::Predicate::ICMP_EQ, new llvm::LoadInst(int32Ty, a, "", entry), b, "", entry);
    llvm::BranchInst::Create(ifBlock, endBlock, cond, entry);

    // If Block
    // {
    //     a = 20
    // }
    new llvm::StoreInst(llvm::ConstantInt::get(int32Ty, 20), a, ifBlock);
    llvm::BranchInst::Create(endBlock, ifBlock);

    // End Block
    // return a
    llvm::ReturnInst::Create(c, new llvm::LoadInst(int32Ty, a, "", endBlock), endBlock);

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

    // Create JIT
    // std::string errStr;
    // llvm::ExecutionEngine *ee =
    //     llvm::EngineBuilder(std::move(owner))
    //     .setErrorStr(&errStr)
    //     .create();

    // if (!ee) {
    //     std::cout << argv[0] << "Failed to construct ExecutionEngine: " << errStr << '\n';
    //     return 1;
    // }

    std::cout << "Verifying .....\n";
    if (llvm::verifyModule(*module)) {
        std::cout << "Error Constructing Function!\n";
        return 1;
    }
    llvm::errs() << *module;

    // std::cout << "Run Function\n";
    
    // // Call Function with argument n:
    // // llvm/ExecutionEngine/GenericValue
    // std::vector<llvm::GenericValue> args(0);
    // // args[0].IntVal = n;
    // llvm::GenericValue gv = ee->runFunction(func, args);

    // // Output
    // llvm::outs() << "Result : " << gv.IntVal << '\n';

    return 0;
}
