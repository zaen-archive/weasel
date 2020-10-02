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


// Simple Variable Declaration and definition
/*
g = 213
func something() {
    a = g
    return a
}
*/
llvm::Function *createFunctionDecDefVar(llvm::Module *m, llvm::LLVMContext &c) {
    // Functions
    llvm::FunctionType *funTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(c), false);
    llvm::Function *fun = llvm::Function::Create(funTy, llvm::Function::LinkageTypes::ExternalLinkage, "test", m);

    llvm::BasicBlock *block = llvm::BasicBlock::Create(c, "entry", fun);

    // Global Variable
    // llvm::PointerType *ptrVal = llvm::PointerType::get(llvm::Type::getInt32Ty(c), 0);
    // llvm::ConstantPointerNull *ptr = llvm::ConstantPointerNull::get(ptrVal);
    llvm::Type *ptrVal = llvm::Type::getInt32Ty(c);
    llvm::Constant *ptr = llvm::ConstantInt::get(ptrVal, 123);
    m->getOrInsertGlobal("we", ptrVal);
    llvm::GlobalVariable *gv = m->getGlobalVariable("we");
    gv->setInitializer(ptr);
    gv->setConstant(false);
    gv->setDSOLocal(true);
    // gv->setExternallyInitialized(true);
    // gv->setLinkage(llvm::GlobalValue::LinkageTypes::WeakAnyLinkage);
    // gv->setAlignment(llvm::MaybeAlign(4));

    // Declare variable
    llvm::AllocaInst *a = new llvm::AllocaInst(llvm::Type::getInt32Ty(c), 0, "a", block);
    
    // Define Some Variable
    llvm::ConstantInt *x = llvm::ConstantInt::get(llvm::Type::getInt32Ty(c), 3);
    llvm::ConstantInt *y = llvm::ConstantInt::get(llvm::Type::getInt32Ty(c), 6);
    llvm::BinaryOperator *z = llvm::BinaryOperator::Create(llvm::Instruction::BinaryOps::Add, x, y, "z", block);

    llvm::StoreInst *gi = new llvm::StoreInst(z, gv, block);

    llvm::StoreInst *i = new llvm::StoreInst(z, a, block);
    llvm::LoadInst *l = new llvm::LoadInst(llvm::Type::getInt32Ty(c), a, "l", block);

    llvm::ReturnInst::Create(c, l, block);

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
    // llvm::Function *func = createFunctionWithSingleBlock(module, context);
    llvm::Function *func = createFunctionDecDefVar(module, context);

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
