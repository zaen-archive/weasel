//
// Created by zaen on 27/06/21.
//
#pragma once

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>

namespace weasel{
    class Passes
    {
    private:
        llvm::legacy::FunctionPassManager *_fpm;

    public:
        explicit Passes(llvm::Module *module) {
            _fpm = new llvm::legacy::FunctionPassManager(module);

            // TODO: Add Unused code passes
            // Do simple "peephole" optimizations and bit-twiddling optzns.
            _fpm->add(llvm::createInstructionCombiningPass());
            // Reassociate expressions.
            _fpm->add(llvm::createReassociatePass());
            // Eliminate Common SubExpressions.
            _fpm->add(llvm::createGVNPass());
            // Simplify the control flow graph (deleting unreachable blocks, etc).
            _fpm->add(llvm::createCFGSimplificationPass());
            // Initialize Function Pass
            _fpm->doInitialization();
        }

        bool run(llvm::Function &fun)
        {
            return _fpm->run(fun);
        }
    };
}
