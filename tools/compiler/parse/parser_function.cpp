#include <llvm/IR/Type.h>
#include "weasel/parse/parser.h"
#include "weasel/ir/context.h"
#include "weasel/symbol/symbol.h"

std::shared_ptr<weasel::Function> weasel::Parser::parsePrallelFunction()
{
    auto parallelType = ParallelType::ParallelFunction;
    if (getNextToken(true)->isKind(TokenKind::TokenKeyKernel))
    {
        parallelType = ParallelType::ParallelKernel;

        getNextToken(true); // eat kernel
    }

    return parseFunction(parallelType);
}

std::shared_ptr<weasel::Function> weasel::Parser::parseFunction(ParallelType parallelType)
{
    auto fun = parseDeclareFunction();
    if (!fun)
    {
        return nullptr;
    }

    if (parallelType == ParallelType::ParallelKernel && !fun->getFunctionType()->getReturnType()->isVoidTy())
    {
        return ErrorTable::addError(getCurrentToken(), "Parallel Kernel just work on void function");
    }

    // Ignore new line
    if (getCurrentToken()->isKind(TokenKind::TokenSpaceNewline))
    {
        getNextToken(true);
    }

    if (!getCurrentToken()->isKind(TokenKind::TokenDelimOpenCurlyBracket))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected {");
    }

    // Set Symbol for parameters and enter a scope
    {
        SymbolTable::enterScope();
        for (const auto &arg : fun->getArgs())
        {
            auto argName = arg->getArgumentName();
            auto ty = arg->getArgumentType();

            auto attrKind = AttributeKind::SymbolVariable;
            if (ty->isPointerTy())
            {
                attrKind = AttributeKind::SymbolPointer;
            }
            else if (ty->isArrayTy())
            {
                attrKind = AttributeKind::SymbolArray;
            }

            auto attr = std::make_shared<Attribute>(argName, AttributeScope::ScopeParam, attrKind, ty);

            SymbolTable::insert(argName, attr);
        }
    }

    auto body = parseFunctionBody();

    // Exit parameter scope
    {
        SymbolTable::exitScope();
        SymbolTable::exitScope();
    }

    if (!body)
    {
        return ErrorTable::addError(getCurrentToken(), "Expected valid body statement!.");
    }

    if (!body->getBody().empty())
    {
        fun->setIsDefine(true);
    }

    fun->setParallelType(parallelType);
    fun->setBody(body);

    return fun;
}

// extern 'fun' identifier '(' args ')' funTy
std::shared_ptr<weasel::Function> weasel::Parser::parseDeclareFunction()
{
    auto isInline = false;
    if (getCurrentToken()->isKind(TokenKind::TokenKeyInline))
    {
        isInline = true;

        getNextToken();
    }

    if (!getCurrentToken()->isKind(TokenKind::TokenKeyFun))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected fun keyword");
    }

    // get next and eat 'fun'
    if (!getNextToken()->isKind(TokenKind::TokenIdentifier))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected an identifier");
    }

    // Check Symbol Table
    auto funIdentifier = getCurrentToken()->getValue();
    if (SymbolTable::get(funIdentifier) != nullptr)
    {
        return ErrorTable::addError(getCurrentToken(), "Function already declared");
    }

    getNextToken(); // eat 'identifier'
    if (!getCurrentToken()->isKind(TokenKind::TokenDelimOpenParen))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected (");
    }

    getNextToken(); // eat '('
    std::vector<std::shared_ptr<weasel::FunctionArgument>> args;
    auto isVararg = false;

    while (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseParen))
    {
        if (isVararg)
        {
            return ErrorTable::addError(getCurrentToken(), "Variable number argument should be final argument");
        }

        auto idenToken = getCurrentToken();
        if (!getCurrentToken()->isKind(TokenKind::TokenIdentifier))
        {
            return ErrorTable::addError(getCurrentToken(), "Expected identifier in function argument");
        }

        auto identifier = getCurrentToken()->getValue();
        if (getNextToken()->isKind(TokenKind::TokenPuncDotThree))
        {
            isVararg = true;
            getNextToken(); // eat ...
        }

        auto *type = parseDataType();
        if (!type)
        {
            return ErrorTable::addError(getCurrentToken(), "Expected type in function argument");
        }

        args.push_back(std::make_shared<FunctionArgument>(idenToken, identifier, type));

        if (!getCurrentToken()->isKind(TokenKind::TokenPuncComma))
        {
            break;
        }

        getNextToken(); // eat ','
    }

    if (!getCurrentToken()->isKind(TokenKind::TokenDelimCloseParen))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected ) in function argument");
    }

    getNextToken(); // eat )

    auto *returnType = parseDataType();
    if (!returnType)
    {
        returnType = llvm::Type::getVoidTy(*getContext());
    }

    auto funTy = std::make_shared<FunctionType>(returnType, args, isVararg);
    auto fun = std::make_shared<Function>(funIdentifier, funTy);

    // Create Symbol for the function
    {
        SymbolTable::insert(funIdentifier, std::make_shared<Attribute>(funIdentifier, AttributeScope::ScopeGlobal, AttributeKind::SymbolFunction, returnType));
    }

    fun->setIsInline(isInline);

    return fun;
}
