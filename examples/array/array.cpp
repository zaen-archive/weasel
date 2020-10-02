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
llvm::Function *createFun(llvm::Module *m, llvm::LLVMContext &c) {

    // Create Function
    // llvm/IR/Function
    llvm::FunctionType *funcTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(c), {}, false);
    llvm::Function *fun = llvm::Function::Create(funcTy, llvm::Function::ExternalLinkage, "fun", m);

    // Create Block
    auto *entry = llvm::BasicBlock::Create(c, "", fun);
    auto* int64Ty = llvm::Type::getInt64Ty(c);
    auto* intTy = llvm::Type::getInt32Ty(c);
    auto* byteTy = llvm::Type::getInt8PtrTy(c);

    //Array Initialize with direct address
    auto numB = 7;
    auto bitCastOps = llvm::Instruction::CastOps::BitCast;
    auto* arrTyB = llvm::ArrayType::get(intTy, numB);
    auto* b = new llvm::AllocaInst(arrTyB, 0, "", entry);
    auto* arrBit = llvm::BitCastInst::Create(bitCastOps, b, byteTy, "", entry);

    // Array Declaration
    auto num = 10;
    auto* arrTy = llvm::ArrayType::get(intTy, num);
    auto* a = new llvm::AllocaInst(arrTy, 0, "", entry);

    // Array Assignment
    auto* el = llvm::GetElementPtrInst::CreateInBounds(arrTy, a, {
        llvm::ConstantInt::get(int64Ty, 0),
        llvm::ConstantInt::get(intTy, 2)
        }, "", entry);
    new llvm::StoreInst(llvm::ConstantInt::get(intTy, 23), el, entry);
    auto* el2 = llvm::GetElementPtrInst::CreateInBounds(arrTy, a, {
        llvm::ConstantInt::get(int64Ty, 0),
        llvm::ConstantInt::get(intTy, 6)
        }, "", entry);
    new llvm::StoreInst(llvm::ConstantInt::get(intTy, 23), el2, entry);
    auto* el3 = llvm::GetElementPtrInst::CreateInBounds(arrTy, a, {
        llvm::ConstantInt::get(int64Ty, 0),
        llvm::ConstantInt::get(intTy, 2)
        }, "", entry);
    auto* lel3 = new llvm::LoadInst(intTy, el3, "", entry);

    // Create Return
    llvm::ReturnInst::Create(c, lel3, entry);
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
    llvm::Function *funAdd = createFun(module, context);

    std::cout << "Verifying .....\n";
    if (llvm::verifyModule(*module)) {
        std::cout << "Error Constructing Function!\n";
        return 1;
    }
    
    llvm::errs() << *module;

    return 0;
}
