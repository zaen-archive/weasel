#pragma once

#include <vector>
#include <memory>
#include "llvm/IR/LLVMContext.h"
#include "weasel/ast/ast.h"
#include "weasel/ir/context.h"

namespace weasel
{
    class Codegen
    {
    private:
        std::unique_ptr<Context> _context;
        std::vector<std::shared_ptr<Function>> _funs;
        std::string _err;
        bool _isParallel;

    public:
        Codegen(std::unique_ptr<Context> context, std::vector<std::shared_ptr<Function>> funs, bool isParallel = false) : _context(std::move(context)), _funs(std::move(funs)), _isParallel(isParallel) {}

        void compile();

    public:
        llvm::Module *getModule() const { return _context->getModule(); }
        llvm::LLVMContext *getContext() const { return _context->getContext(); }
        std::string getError() const { return _err; }

    };

} // namespace weasel
