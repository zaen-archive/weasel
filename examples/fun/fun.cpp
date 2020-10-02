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


/*
fun(a) {
    final b = 23
    int c = b + a
    return b
}
*/
llvm::Function *createFunAdd(llvm::Module *m, llvm::LLVMContext &context) {

    // Create Function
    // llvm/IR/Function
    llvm::FunctionType *funcTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), {
        llvm::Type::getInt32Ty(context)
        }, false);
    llvm::Function *fun = llvm::Function::Create(funcTy, llvm::Function::ExternalLinkage, "fun_add", m);

    // Create Block
    auto *entry = llvm::BasicBlock::Create(context, "", fun);
    auto *a = fun->args().begin();
    auto *b = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 23);
    auto* c = llvm::BinaryOperator::Create(llvm::Instruction::BinaryOps::Add, a, b, "", entry);

    // Create Return
    llvm::ReturnInst::Create(context, c, entry);
    return fun;
}


/*
fun() {
entry:
    b = 123
    c = block2() + b
    return c
}
*/
llvm::Function *createFunCall(llvm::Module *m, llvm::LLVMContext &context, llvm::FunctionCallee fun) {
    // Create Function
    // llvm/IR/Function
    llvm::FunctionType *funcTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), {}, false);
    llvm::Function *func = llvm::Function::Create(funcTy, llvm::Function::ExternalLinkage, "fun_call", m);

    // Types
    auto *int32Ty = llvm::Type::getInt32Ty(context);

    // Create Block
    auto *entry = llvm::BasicBlock::Create(context, "", func);
    auto* callAdd = llvm::CallInst::Create(fun, {
        llvm::ConstantInt::get(int32Ty, 23)
        }, "", entry);
    auto *retVal = llvm::ConstantInt::get(int32Ty, 23);
    auto* ret = llvm::BinaryOperator::Create(llvm::Instruction::BinaryOps::Add, callAdd, retVal, "", entry);

    // Create Return
    llvm::ReturnInst::Create(context, ret, entry);

    return func;
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
    llvm::Function *funAdd = createFunAdd(module, context);
    llvm::Function *funCall = createFunCall(module, context, funAdd);

    std::cout << "Verifying .....\n";
    if (llvm::verifyModule(*module)) {
        std::cout << "Error Constructing Function!\n";
        return 1;
    }
    
    llvm::errs() << *module;

    return 0;
}
