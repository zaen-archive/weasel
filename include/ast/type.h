#pragma once

#include "llvm/IR/type.h"

namespace underrated
{
    class AnalysContext;

    class Type
    {
    protected:
        unsigned _valueWidth;
        bool _valueSigned;

        Type(unsigned valueWidth, bool isSigned) : _valueWidth(valueWidth), _valueSigned(isSigned) {}

    public:
        virtual llvm::Type *codegen(AnalysContext *c) = 0;
    };

    class IntType : public Type
    {
    private:
        IntType(unsigned valueWidth, bool isSigned) : Type(valueWidth, isSigned) {}

    public:
        static IntType *getInt8Ty(bool isSigned) { return new IntType(8, isSigned); }
        static IntType *getInt16Ty(bool isSigned) { return new IntType(16, isSigned); }
        static IntType *getInt32Ty(bool isSigned) { return new IntType(32, isSigned); }
        static IntType *getInt64Ty(bool isSigned) { return new IntType(64, isSigned); }
        static IntType *getInt128Ty(bool isSigned) { return new IntType(128, isSigned); }

        llvm::Type *codegen(AnalysContext *c);
    };

    class VoidType : public Type
    {
    public:
        VoidType() : Type(0, false) {}

        llvm::Type *codegen(AnalysContext *c);
    };

    class BoolType : public Type
    {
    public:
        BoolType() : Type(1, false) {}

        llvm::Type *codegen(AnalysContext *c);
    };

    class FloatType : public Type
    {
    private:
        FloatType(unsigned valueWidth, bool isSigned) : Type(valueWidth, isSigned) {}

    public:
        static FloatType *getFloatTy() { return new FloatType(32, true); }
        static FloatType *getDoubleTy() { return new FloatType(64, true); }
        static FloatType *getDecimalTy() { return new FloatType(128, true); }

        llvm::Type *codegen(AnalysContext *c);
    };

    class FuncArg
    {
    private:
        std::string _argName;
        Type *_type;

    public:
        FuncArg(std::string argName, Type *type) : _argName(argName), _type(type) {}

        Type *getType() const { return _type; }
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
