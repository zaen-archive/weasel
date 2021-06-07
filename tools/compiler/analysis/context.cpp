#include <iostream>
#include "llvm/IR/Function.h"
#include "zero/analysis/context.h"
#include "zero/symbol/symbol.h"

zero::AnalysContext::AnalysContext()
{
    _context = new llvm::LLVMContext();
    _module = new llvm::Module("zero", *getContext());
    _builder = new llvm::IRBuilder<>(*getContext());
}

zero::AnalysContext::AnalysContext(std::string moduleName)
{
    _context = new llvm::LLVMContext();
    _module = new llvm::Module(moduleName, *getContext());
    _builder = new llvm::IRBuilder<>(*getContext());
}

std::string zero::AnalysContext::getDefaultLabel()
{
    return std::to_string(_counter++);
}

llvm::Function *zero::AnalysContext::codegen(Function *funAST)
{
    auto funName = funAST->getIdentifier();
    if (getModule()->getFunction(funName))
    {
        return nullptr;
    }

    auto funArgs = funAST->getArgs();
    auto *retTy = funAST->getFunctionType()->getReturnType();
    auto args = std::vector<llvm::Type *>();
    if (funArgs.size() > 0)
    {
        for (auto &item : funArgs)
        {
            args.push_back(item->getArgumentType());
        }
    }

    auto *funTyLLVM = llvm::FunctionType::get(retTy, args, false);
    auto *funLLVM = llvm::Function::Create(funTyLLVM, llvm::Function::ExternalWeakLinkage, funName, *getModule());
    auto idx = 0;
    for (auto &item : funLLVM->args())
    {
        item.setName(funArgs[idx++]->getArgumentName());
    }

    // Add Function to symbol table
    {
        auto *attr = new Attribute(funName, AttributeScope::ScopeGlobal, AttributeKind::SymbolFunction, funLLVM);

        SymbolTable::getInstance().insert(funName, attr);
    }

    if (funAST->getIsDefine())
    {
        // Add Parameter to symbol table
        {
            // Enter to parameter scope
            SymbolTable::getInstance().enterScope();

            for (auto &item : funLLVM->args())
            {
                auto refName = item.getName();
                auto paramName = std::string(refName.begin(), refName.end());
                auto *attr = new Attribute(paramName, AttributeScope::ScopeParam, AttributeKind::SymbolParameter, &item);

                SymbolTable::getInstance().insert(paramName, attr);
            }
        }

        auto *entry = llvm::BasicBlock::Create(*getContext(), "entry", funLLVM);
        getBuilder()->SetInsertPoint(entry);

        // Create Block
        if (funAST->getBody())
        {
            funAST->getBody()->codegen(this);
        }

        // Exit from parameter scope
        {
            auto exit = SymbolTable::getInstance().exitScope();
            if (!exit)
            {
                return nullptr;
            }
        }
    }

    return funLLVM;
}

llvm::Value *zero::AnalysContext::codegen(StatementExpression *expr)
{
    std::cout << "Statements : " << expr->getBody().size() << "\n";

    // Enter to new statement
    {
        SymbolTable::getInstance().enterScope();
    }

    for (auto &item : expr->getBody())
    {
        item->codegen(this);
    }

    // Exit from statement
    {
        SymbolTable::getInstance().exitScope();
    }

    return nullptr;
}

llvm::Value *zero::AnalysContext::codegen(NumberLiteralExpression *expr)
{
    return getBuilder()->getInt32(expr->getValue());
}

llvm::Value *zero::AnalysContext::codegen(DeclarationExpression *expr)
{
    // Get Value Representation
    auto *value = expr->getValue()->codegen(this);

    // Checking Type
    auto declTy = expr->getType() != nullptr ? expr->getType() : value->getType();
    auto compareTy = compareType(declTy, value->getType());
    if (compareTy == CompareType::Different)
    {
        return logErrorV(std::string("Cannot assign, expression type is different"));
    }
    if (compareTy == CompareType::Casting)
    {
        value = castIntegerType(value, declTy);
    }

    // Allocating Address for declaration
    auto varName = expr->getIdentifier();
    auto *alloc = getBuilder()->CreateAlloca(declTy, 0, varName);

    // Add Variable Declaration to symbol table
    {
        auto *attr = new Attribute(varName, AttributeScope::ScopeLocal, AttributeKind::SymbolVariable, alloc);

        SymbolTable::getInstance().insert(varName, attr);
    }

    return getBuilder()->CreateStore(value, alloc);
}

llvm::Value *zero::AnalysContext::codegen(BinaryOperatorExpression *expr)
{
    auto *token = expr->getOperator();
    auto *rhs = expr->getRHS()->codegen(this);
    auto *lhs = expr->getLHS()->codegen(this);
    auto compareTy = compareType(lhs->getType(), rhs->getType());

    if (compareTy == CompareType::Different)
    {
        return logErrorV(std::string("type LHS != type RHS"));
    }

    if (compareTy == CompareType::Casting)
    {
        castIntegerType(lhs, rhs);
    }

    switch (token->getTokenKind())
    {
    case TokenKind::TokenOperatorStar:
        return getBuilder()->CreateMul(lhs, rhs, "multiply");
    case TokenKind::TokenOperatorSlash:
        return getBuilder()->CreateSDiv(lhs, rhs, "division");
    // case TokenKind::TokenPuncPercent: return llvm::BinaryOperator::
    case TokenKind::TokenOperatorPlus:
        return getBuilder()->CreateAdd(lhs, rhs, "addition");
    case TokenKind::TokenOperatorMinus:
        return getBuilder()->CreateSub(lhs, rhs, "subtraction");
    case TokenKind::TokenOperatorEqual:
    {
        auto *loadLhs = llvm::dyn_cast<llvm::LoadInst>(lhs);
        if (!loadLhs)
        {
            return ErrorTable::addError(new Error(expr->getLHS()->getToken(), "LHS not valid"));
        }

        auto *allocLhs = llvm::dyn_cast<llvm::AllocaInst>(loadLhs->getPointerOperand());
        if (!allocLhs)
        {
            return ErrorTable::addError(new Error(expr->getLHS()->getToken(), "LHS is not a valid address pointer"));
        }

        getBuilder()->CreateStore(rhs, allocLhs);
        return getBuilder()->CreateLoad(allocLhs);
    }
    default:
        return nullptr;
    }
}

// Remove it in future
llvm::Value *zero::AnalysContext::codegen(AssignmentExpression *expr)
{
    auto *lhs = expr->getLHS()->codegen(this);
    if (!lhs->getType()->isPointerTy())
    {
        return logErrorV(std::string("LHS should be a pointer to an address"));
    }

    auto *rhs = expr->getRHS()->codegen(this);
    auto compareTy = compareType(lhs->getType()->getContainedType(0), rhs->getType());
    if (compareTy == CompareType::Different)
    {
        return logErrorV(std::string("Cannot assign, expression type is different"));
    }

    if (compareTy == CompareType::Casting)
    {
        rhs = castIntegerType(rhs, lhs->getType()->getContainedType(0));
    }

    return getBuilder()->CreateStore(rhs, lhs);
}

llvm::Value *zero::AnalysContext::codegen(ReturnExpression *expr)
{
    if (!expr->getValue())
    {
        return getBuilder()->CreateRetVoid();
    }

    auto *val = expr->getValue()->codegen(this);

    // Get Last Function from symbol table
    auto *funAttr = SymbolTable::getInstance().getLastFunction();
    if (!funAttr)
    {
        return ErrorTable::addError(new Error(expr->getToken(), "Return Statement cannot find last function from symbol table"));
    }
    auto *fun = llvm::dyn_cast<llvm::Function>(funAttr->getValue());
    auto *returnTy = fun->getReturnType();
    auto compareTy = compareType(returnTy, val->getType());

    if (compareTy == CompareType::Different)
    {
        return ErrorTable::addError(new Error(expr->getToken(), "Return Type with value type is different"));
    }

    if (compareTy == CompareType::Casting)
    {
        val = castIntegerType(val, returnTy);
    }

    return getBuilder()->CreateRet(val);
}

llvm::Value *zero::AnalysContext::codegen(VariableExpression *expr)
{
    // Get Allocator from Symbol Table
    auto varName = expr->getIdentifier();
    auto *alloc = SymbolTable::getInstance().get(varName);
    if (!alloc)
    {
        return ErrorTable::addError(new Error(expr->getToken(), "Variable " + varName + " Not declared"));
    }

    return getBuilder()->CreateLoad(alloc->getValue(), varName);
}

/// HELPER ///
// Compare Type Helpter
zero::CompareType zero::AnalysContext::compareType(llvm::Type *lhsType, llvm::Type *rhsType)
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

// Cast Integer Type Helper
llvm::Value *zero::AnalysContext::castIntegerType(llvm::Value *value, llvm::Type *castTy)
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

// Cast Integer Type Helper
llvm::Value *zero::AnalysContext::castIntegerType(llvm::Value *lhs, llvm::Value *rhs)
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
