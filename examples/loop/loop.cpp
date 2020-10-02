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

// bool a = true
// while a
// {
//     a = false
// }
//
// int b = 1
// for let i = 0; i < 10; i++
// {
//     b = b * i
// }
// return 0
llvm::Function *createFun(llvm::Module *m, llvm::LLVMContext &c) {
    // Default Type
    auto *int32Ty = llvm::Type::getInt32Ty(c);
    auto *boolTy = llvm::Type::getInt8Ty(c);
    auto *bitType = llvm::Type::getInt1Ty(c);

    // Functions
    llvm::FunctionType *funTy = llvm::FunctionType::get(int32Ty, false);
    llvm::Function *fun = llvm::Function::Create(funTy, llvm::Function::LinkageTypes::ExternalLinkage, "test", m);

    // Init Block
    auto *entry = llvm::BasicBlock::Create(c, "", fun);
    auto *endBlock = llvm::BasicBlock::Create(c, "", fun);

    // Align Value
    auto *retVal = new llvm::AllocaInst(int32Ty, 0, "", entry);
    new llvm::StoreInst(llvm::ConstantInt::get(int32Ty, 0), retVal, entry);

    /// Your Code ///
    // bool a = true
    auto *a = new llvm::AllocaInst(bitType, 0, "", entry);
    new llvm::StoreInst(llvm::ConstantInt::get(bitType, 1), a, entry);

    // Initialize for block For [bool] { [statement] }
    auto *whileCond = llvm::BasicBlock::Create(c, "", fun);
    auto *whileBlock = llvm::BasicBlock::Create(c, "", fun);
    auto *postWhile = llvm::BasicBlock::Create(c, "", fun);
    llvm::BranchInst::Create(whileCond, entry);
    
    // for a {
    //     a = false
    // }
    // For Check
    llvm::BranchInst::Create(whileBlock, postWhile, new llvm::LoadInst(bitType, a,  "", whileCond), whileCond);
    // For Block
    new llvm::StoreInst(llvm::ConstantInt::get(bitType, 0), a, whileBlock);
    llvm::BranchInst::Create(whileCond, whileBlock);

    // Initialize for [init]; [condition]; [increment] { [Statement] }
    auto *forCond = llvm::BasicBlock::Create(c, "", fun);
    auto *forIncre = llvm::BasicBlock::Create(c, "", fun);
    auto *forBlock = llvm::BasicBlock::Create(c, "", fun);
    // [init]
    // int b = 1
    // for int i = 0; i < 10; i++
    auto *b = new llvm::AllocaInst(int32Ty, 0, "", postWhile);
    auto *i = new llvm::AllocaInst(int32Ty, 0, "", postWhile);
    new llvm::StoreInst(llvm::ConstantInt::get(int32Ty, 1), b, postWhile);
    new llvm::StoreInst(llvm::ConstantInt::get(int32Ty, 0), i, postWhile);
    llvm::BranchInst::Create(forCond, postWhile);

    // [condition]
    // for int i = 0; i < 10; i++
    auto *condTo = llvm::ConstantInt::get(int32Ty, 10);
    auto *cond = llvm::ICmpInst::Create(llvm::Instruction::OtherOps::ICmp,
        llvm::ICmpInst::Predicate::ICMP_SLT, new llvm::LoadInst(int32Ty, i,
        "", forCond), condTo, "", forCond);
    llvm::BranchInst::Create(forBlock, endBlock, cond, forCond);

    // [Statement]
    // for []; []; [] { b = b * i }
    auto *bMul = llvm::BinaryOperator::Create(llvm::Instruction::BinaryOps::Mul,
        new llvm::LoadInst(int32Ty, b, "", forBlock), new llvm::LoadInst(int32Ty, i, "", forBlock), "", forBlock);
    new llvm::StoreInst(bMul, b, forBlock);
    llvm::BranchInst::Create(forIncre, forBlock);

    // [increment]
    auto *incre = llvm::BinaryOperator::Create(llvm::Instruction::BinaryOps::Add,
        new llvm::LoadInst(int32Ty, i, "", forIncre), llvm::ConstantInt::get(int32Ty, 1), "", forIncre);
    new llvm::StoreInst(incre, i, forIncre);
    llvm::BranchInst::Create(forCond, forIncre);

    /// End Your Code ///

    // End Block
    // return a
    llvm::ReturnInst::Create(c, new llvm::LoadInst(int32Ty, retVal, "", endBlock), endBlock);

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
