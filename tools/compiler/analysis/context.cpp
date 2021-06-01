#include <iostream>
#include "analysis/context.h"

underrated::AnalysContext::AnalysContext()
{
    _context = new llvm::LLVMContext();
    _module = new llvm::Module("underrated", *getContext());
    _builder = new llvm::IRBuilder<>(*getContext());
}

underrated::AnalysContext::AnalysContext(std::string moduleName)
{
    _context = new llvm::LLVMContext();
    _module = new llvm::Module(moduleName, *getContext());
    _builder = new llvm::IRBuilder<>(*getContext());
}

std::string underrated::AnalysContext::getDefaultLabel()
{
    return std::to_string(_counter++);
}

llvm::Function *underrated::AnalysContext::codegen(Function *func)
{
    if (getModule()->getFunction(func->getIdentifier()))
    {
        return nullptr;
    }

    auto funcArgs = func->getArgs();
    auto *retTy = func->getFunctionType()->getReturnType();
    auto args = std::vector<llvm::Type *>();
    if (funcArgs.size() > 0)
    {
        for (auto &item : funcArgs)
        {
            args.push_back(item->getArgumentType());
        }
    }

    auto *funcTy = llvm::FunctionType::get(retTy, args, false);
    auto *f = llvm::Function::Create(funcTy, llvm::Function::ExternalWeakLinkage, func->getIdentifier(), *getModule());
    auto idx = 0;
    for (auto &item : f->args())
    {
        item.setName(funcArgs[idx++]->getArgumentName());
    }

    if (func->getIsDefine())
    {
        auto *entry = llvm::BasicBlock::Create(*getContext(), "entry", f);
        getBuilder()->SetInsertPoint(entry);

        // Create Block
        if (func->getBody())
        {
            func->getBody()->codegen(this);
        }
    }

    return f;
}

llvm::Value *underrated::AnalysContext::codegen(StatementExpression *expr)
{
    std::cout << "Statements : " << expr->getBody().size() << "\n";
    for (auto &item : expr->getBody())
    {
        item->codegen(this);
    }

    return nullptr;
}

llvm::Value *underrated::AnalysContext::codegen(NumberLiteralExpression *expr)
{
    return getBuilder()->getInt32(expr->getValue());
}

llvm::Value *underrated::AnalysContext::codegen(DeclarationExpression *expr)
{
    return nullptr;
}

underrated::CompareType underrated::AnalysContext::compareType(llvm::Type *lhsType, llvm::Type *rhsType)
{
    if (lhsType->getTypeID() != rhsType->getTypeID())
    {
        return CompareType::Different;
    }

    if (lhsType->isIntegerTy())
    {
        if (lhsType->getIntegerBitWidth() != rhsType->getIntegerBitWidth())
        {
            return CompareType::Casting;
        }
    }

    return CompareType::Equal;
}

llvm::Value *underrated::AnalysContext::castIntegerType(llvm::Value *value, llvm::Type *castTy)
{
    if (value->getType()->getIntegerBitWidth() < castTy->getIntegerBitWidth())
    {
        return getBuilder()->CreateCast(llvm::Instruction::CastOps::SExt, value, castTy);
    }
    else
    {
        return getBuilder()->CreateCast(llvm::Instruction::CastOps::Trunc, value, castTy);
    }
}

llvm::Value underrated::AnalysContext::castIntegerType(llvm::Value *lhs, llvm::Value *rhs)
{
    if (lhs->getType()->getIntegerBitWidth() > rhs->getType()->getIntegerBitWidth())
    {
        return getBuilder()->CreateCast(llvm::Instruction::CastOps::SExt, rhs, lhs->getType());
    }
    else
    {
        return getBuilder()->CreateCast(llvm::Instruction::CastOps::SExt, lhs, rhs->getType());
    }
}

llvm::Value *underrated::AnalysContext::codegen(BinaryOperatorExpression *expr)
{
    auto *token = expr->getOperator();
    auto *rhs = expr->getRHS()->codegen(this);
    auto *lhs = expr->getLHS()->codegen(this);
    auto compareTy = compareType(lhs->getType(), rhs->getType());

    if (compareTy == CompareType::Different)
    {
        return logErrorV("type LHS != type RHS");
    }

    if (compareTy == CompareType::Casting)
    {
        castIntegerType(lhs, rhs);
    }

    switch (token->getTokenKind())
    {
    case TokenKind::TokenPuncStar:
        return getBuilder()->CreateMul(lhs, rhs, "multiply");
    case TokenKind::TokenPuncSlash:
        return getBuilder()->CreateSDiv(lhs, rhs, "division");
    // case TokenKind::TokenPuncPercent: return llvm::BinaryOperator::
    case TokenKind::TokenPuncPlus:
        return getBuilder()->CreateAdd(lhs, rhs, "addition");
    case TokenKind::TokenPuncMinus:
        return getBuilder()->CreateSub(lhs, rhs, "subtraction");
    default:
        return nullptr;
    }
}

llvm::Value *underrated::AnalysContext::codegen(AssignmentExpression *expr)
{
    auto *lhs = expr->getLHS()->codegen(this);
    if (!lhs->getType()->isPointerTy())
    {
        return logErrorV("LHS should be a pointer to an address");
    }

    auto *rhs = expr->getRHS()->codegen(this);
    auto compareTy = compareType(lhs->getType()->getContainedType(0), rhs->getType());

    // rhs = getBuilder()->CreateCast(llvm::Instruction::CastOps::SExt, rhs, getBuilder()->getInt64Ty());

    if (compareTy == CompareType::Different)
    {
        return logErrorV("Cannot assign, expression type is different");
    }

    if (compareTy == CompareType::Casting)
    {
        std::cout << "Casting\n";
        rhs = castIntegerType(rhs, lhs->getType()->getContainedType(0));
    }

    return getBuilder()->CreateStore(rhs, lhs);
}

llvm::Value *underrated::AnalysContext::codegen(ReturnExpression *expr)
{
    if (expr->getValue())
    {
        return getBuilder()->CreateRet(expr->getValue()->codegen(this));
    }
    else
    {
        return getBuilder()->CreateRetVoid();
    }
}

// TODO: Handle just definition variable
llvm::Value *underrated::AnalysContext::codegen(VariableExpression *expr)
{
    auto *val = getBuilder()->CreateAlloca(getBuilder()->getInt64Ty(), 0, expr->getIdentifier());
    return val;
}
