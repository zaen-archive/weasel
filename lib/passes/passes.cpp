//
// Created by zaen on 29/06/21.
//
#include "weasel/passes/passes.h"

weasel::Passes::Passes(llvm::Module *module)
{
    _fpm = new llvm::legacy::FunctionPassManager(module);

    // TODO: Add Unused code passes
    _fpm->add(llvm::createConstraintEliminationPass());   // Propagate constant
    _fpm->add(llvm::createPromoteMemoryToRegisterPass()); // SSA conversion
    _fpm->add(llvm::createDeadCodeEliminationPass());     // Dead code elimination
    _fpm->add(llvm::createSROAPass());                    // SROA
    _fpm->add(llvm::createLoopSimplifyCFGPass());         // Simplify Loop
    _fpm->add(llvm::createNewGVNPass());                  // Global value numbering
    _fpm->add(llvm::createReassociatePass());             // Reassociate expressions.
    _fpm->add(llvm::createPartiallyInlineLibCallsPass()); // Inline standard calls
    _fpm->add(llvm::createCFGSimplificationPass());       // Cleanup
    _fpm->add(llvm::createInstructionCombiningPass());    // Do simple "peephole" optimizations and bit-twiddling optzns.
    _fpm->add(llvm::createFlattenCFGPass());              // Flatten the control flow graph.
    _fpm->add(llvm::createGVNPass());                     // Eliminate Common SubExpressions.

    // Initialize Function Pass
    _fpm->doInitialization();
}

bool weasel::Passes::run(llvm::Function &fun)
{
    return _fpm->run(fun);
}
