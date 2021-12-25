#pragma once

#include <vector>
#include <memory>
#include "llvm/IR/LLVMContext.h"
#include "weasel/ast/ast.h"
#include "weasel/ir/context.h"
#include "weasel/config/config.h"

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
        Codegen(std::unique_ptr<Context> context, std::vector<std::shared_ptr<Function>> funs);

        bool compile(const std::string &spirvIR = "");

        std::string createSpirv();
        void createObject(char *outputFile) const;

    public:
        llvm::Module *getModule() const { return _context->getModule(); }
        llvm::LLVMContext *getContext() const { return _context->getContext(); }

        std::string getError() const { return _err; }
    };

} // namespace weasel
