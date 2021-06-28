//
// Created by zaen on 27/06/21.
//
#pragma once

#include <llvm/IR/Module.h>
#include <llvm/IR/Metadata.h>
#include <llvm/IR/MDBuilder.h>

namespace weasel
{
    class Metadata
    {
    private:
        llvm::MDBuilder *_builder;
        llvm::LLVMContext *_context;

    private:
        llvm::Metadata *getFlagsMetadata();
        llvm::Metadata *getVersionMetadata();
        llvm::MDNode *getCLVersionMetadata();

        llvm::MDBuilder *getBuilder() const { return _builder; }
        llvm::LLVMContext *getContext() const { return _context; }

    public:
        explicit Metadata(llvm::LLVMContext *context);

        void initParallelModule(llvm::Module *module) {
            initModule(module);

            module->getOrInsertNamedMetadata("opencl.ocl.version")->addOperand(getCLVersionMetadata());
            module->getOrInsertNamedMetadata("opencl.spir.version")->addOperand(getCLVersionMetadata());
        }

        void initModule(llvm::Module *module)
        {
            module->addModuleFlag(llvm::Module::ModFlagBehavior::ModFlagBehaviorFirstVal, "wchar_size", getFlagsMetadata());
            module->getOrInsertNamedMetadata("llvm.ident")->addOperand(llvm::MDNode::get(module->getContext(), {getVersionMetadata()}));
        }
    };
}
