#pragma once

#include "llvm/IR/type.h"

namespace underrated
{
    enum class Qualifier
    {
        QualConst,
        QualRestrict,
        QualVolatile,
    };

    // Type
    // llvm type
    // Qualifier
    // codegen
    class Type
    {
    private:
        llvm::Type *_llvmType;
        Qualifier _qual;
        bool _isSigned;

    public:
        Type(llvm::Type *llvmType, Qualifier qualifier = Qualifier::QualVolatile, bool isSigned = true) : _llvmType(llvmType), _qual(qualifier), _isSigned(isSigned) {}

        llvm::Type *getLLVMType() const { return _llvmType; }
    };

    class FuncArg
    {
    private:
        std::string _argName;
        Type *_type;

    public:
        FuncArg(std::string argName, Type *type) : _argName(argName), _type(type) {}

        Type *getType() const { return _type; }
        llvm::Type *getLLVMType() const { return _type->getLLVMType(); }
        std::string getArgName() const { return _argName; }
    };

    class FuncType
    {
    private:
        std::vector<FuncArg *> _args;
        Type *_type;

    public:
        FuncType(Type *returnType, std::vector<FuncArg *> args) : _type(returnType), _args(args) {}

        std::vector<FuncArg *> getArgs() const { return _args; }
        Type *getType() const { return _type; }
    };
} // namespace underrated
