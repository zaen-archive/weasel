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
    let a = 213
    var b = 432
    var d = a * b
    return d
}
*/
llvm::Function *createFun(llvm::Module *m, llvm::LLVMContext &c) {
    // Functions
    llvm::FunctionType *funTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(c), false);
    llvm::Function *fun = llvm::Function::Create(funTy, llvm::Function::LinkageTypes::ExternalLinkage, "test", m);

    llvm::BasicBlock *block = llvm::BasicBlock::Create(c, "entry", fun);
    llvm::Type *intTy = llvm::Type::getInt32Ty(c);

    // let a = 213
    llvm::Constant *a = llvm::ConstantInt::get(intTy, 213);

    // var b = 432
    llvm::AllocaInst *b = new llvm::AllocaInst(intTy, 0, "b", block);
    llvm::StoreInst *sb = new llvm::StoreInst(llvm::ConstantInt::get(intTy, 432), b, block);

    // var d = a * b
    llvm::AllocaInst *d = new llvm::AllocaInst(intTy, 0, "d", block);
    llvm::LoadInst *lb = new llvm::LoadInst(intTy, b, "b", block);
    // 'a' is a constant Just reuse it.
    llvm::BinaryOperator *dOp = llvm::BinaryOperator::CreateMul(a, lb, "d_op", block);
    new llvm::StoreInst(dOp, d, block);
    llvm::LoadInst *ld = new llvm::LoadInst(intTy, d, "d", block);

    llvm::ReturnInst::Create(c, ld, block);

    return fun;
}


int main(int argc, char **argv) {
    // Argument
    int n = argc > 1 ? atol(argv[1]) : 24;

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
    std::string errStr;
    llvm::ExecutionEngine *ee =
        llvm::EngineBuilder(std::move(owner))
        .setErrorStr(&errStr)
        .create();

    if (!ee) {
        std::cout << argv[0] << "Failed to construct ExecutionEngine: " << errStr << '\n';
        return 1;
    }

    std::cout << "Verifying .....\n";
    if (llvm::verifyModule(*module)) {
        std::cout << "Error Constructing Function!\n";
        return 1;
    }
    
    llvm::errs() << *module;

    std::cout << "Run Function\n";
    
    // Call Function with argument n:
    // llvm/ExecutionEngine/GenericValue
    std::vector<llvm::GenericValue> args(0);
    // args[0].IntVal = n;
    llvm::GenericValue gv = ee->runFunction(func, args);

    // Output
    llvm::outs() << "Result : " << gv.IntVal << '\n';

    return 0;
}
