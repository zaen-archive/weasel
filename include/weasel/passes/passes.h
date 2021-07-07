//
// Created by zaen on 27/06/21.
//
#pragma once

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/Transforms/Utils.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/Analysis/TargetTransformInfo.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

namespace weasel
{
    class Passes
    {
    private:
        llvm::legacy::FunctionPassManager *_fpm;

    public:
        explicit Passes(llvm::Module *module)
        {
            _fpm = new llvm::legacy::FunctionPassManager(module);

            // TODO: Add Unused code passes
            _fpm->add(llvm::createGVNPass());                     // Eliminate Common SubExpressions.
            _fpm->add(llvm::createPromoteMemoryToRegisterPass()); // SSA conversion
            _fpm->add(llvm::createDeadCodeEliminationPass());     // Dead code elimination
            _fpm->add(llvm::createSROAPass());                    // SROA
            _fpm->add(llvm::createLoopSimplifyCFGPass());         // Simplify Loop
            _fpm->add(llvm::createConstantPropagationPass());     // Propagate constant
            _fpm->add(llvm::createNewGVNPass());                  // Global value numbering
            _fpm->add(llvm::createReassociatePass());             // Reassociate expressions.
            _fpm->add(llvm::createPartiallyInlineLibCallsPass()); // Inline standard calls
            _fpm->add(llvm::createCFGSimplificationPass());       // Cleanup
            _fpm->add(llvm::createInstructionCombiningPass());    // Do simple "peephole" optimizations and bit-twiddling optzns.
            _fpm->add(llvm::createFlattenCFGPass());              // Flatten the control flow graph.

            // Initialize Function Pass
            _fpm->doInitialization();
        }

        bool run(llvm::Function &fun)
        {
            return _fpm->run(fun);
        }
    };
}
