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
#include <iostream>
#include <memory>
#include "parse/parser.h"
#include "ast/ast_context.h"

int main()
{
    // Initialize LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    auto *lexer = new underrated::Lexer();
    auto *parser = new underrated::Parser(lexer);
    auto *context = new underrated::ASTContext();

    /// Compiler ///
    while (true)
    {
        std::cout << "Typing > ";
        auto *expr = parser->parse();

        if (expr)
        {
            expr->debug();
            expr->codegen(context);
        }
        else
        {
            std::cout << "\n";
        }
    }

    // testing s//
    // testing();
}

int testing()
{
    auto *context = new underrated::ASTContext();
    auto *builder = context->getBuilder();
    auto *funTy = llvm::FunctionType::get(builder->getInt32Ty(), false);
    auto *fun = llvm::Function::Create(funTy, llvm::Function::ExternalLinkage, "testing", context->getModule());
    auto *block = llvm::BasicBlock::Create(context->getContext(), "entry", fun);

    builder->SetInsertPoint(block);

    auto *str = builder->CreateGlobalStringPtr("Hello World", "msg");
    std::vector<llvm::Value *> callParams;
    callParams.push_back(str);

    // Init printf
    auto *pty = llvm::PointerType::get(llvm::IntegerType::get(context->getContext(), 8), 0);
    auto *fty = llvm::FunctionType::get(llvm::IntegerType::get(context->getContext(), 32), true);
    auto *fpf = llvm::Function::Create(fty, llvm::GlobalValue::ExternalLinkage, "puts", context->getModule());

    auto test = [&]()
    {
        // int Type
        auto *b = new llvm::AllocaInst(builder->getInt32Ty(), 0, "b", block);
        auto *sb = new llvm::StoreInst(llvm::ConstantInt::get(builder->getInt32Ty(), 432), b, block);

        auto *funPrintf = context->getModule()->getFunction("puts");
        if (!funPrintf)
        {
            std::cout << "Printf not exist\n";
        }
        else
        {
            auto *calling = llvm::CallInst::Create(funPrintf, callParams, "callthing", block);
            std::cout << "Printf exist\n";
        }
    };

    auto end = [&]()
    {
        llvm::ReturnInst::Create(context->getContext(), llvm::ConstantInt::get(builder->getInt32Ty(), 10), block);
    };

    test();
    end();

    // Verify Function
    llvm::verifyFunction(*fun);

    // Create JIT
    std::string errMsg;
    llvm::ExecutionEngine *ee = llvm::EngineBuilder(std::move(std::unique_ptr<llvm::Module>(context->getModule()))).setErrorStr(&errMsg).create();
    if (!ee)
    {
        std::cout << "Failed to construct ExecutionEngine: " << errMsg << '\n';
        return 1;
    }

    // Verifying Module
    std::cout << "Verifying ...\n";
    if (llvm::verifyModule(*context->getModule()))
    {
        std::cout << "Error Constructing Function!\n";
        return 1;
    }

    // Show Module
    llvm::errs() << *context->getModule();

    std::cout << "Run Function\n";
    std::vector<llvm::GenericValue> args(0);
    llvm::GenericValue gv = ee->runFunction(fun, args);

    llvm::outs() << "Result : " << gv.IntVal << "\n";
    return 0;
}