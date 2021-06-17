#include "llvm/IR/Type.h"
#include "llvm/IR/Instructions.h"
#include "zero/parse/parser.h"
#include "zero/analysis/context.h"
#include "zero/symbol/symbol.h"

// define
// 'fun' identifier '(' args ')' funTy '{' stmt '}'
std::shared_ptr<weasel::Function> weasel::Parser::parseFunction()
{
    auto func = parseDeclareFunction();
    if (!func)
    {
        return nullptr;
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
        SymbolTable::getInstance().enterScope();
        for (auto arg : func->getArgs())
        {
            auto argName = arg->getArgumentName();
            auto ty = arg->getArgumentType();
            auto attr = std::make_shared<Attribute>(argName, AttributeScope::ScopeParam, AttributeKind::SymbolParameter, ty);

            SymbolTable::getInstance().insert(argName, attr);
        }
    }

    auto body = parseFunctionBody();

    // Exit parameter scope
    {
        SymbolTable::getInstance().exitScope();
    }

    if (!body)
    {
        return ErrorTable::addError(getCurrentToken(), "Expected valid body statement!.");
    }

    if (!body->getBody().empty())
    {

        func->setIsDefine(true);
    }

    func->setBody(body);
    return func;
}

// extern 'fun' identifier '(' args ')' funTy
std::shared_ptr<weasel::Function> weasel::Parser::parseDeclareFunction()
{
    if (!getCurrentToken()->isKind(TokenKind::TokenKeyFun))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected fun keyword");
    }

    // get next and eat 'fun'
    if (!getNextToken()->isKind(TokenKind::TokenIdentifier))
    {
        return ErrorTable::addError(getCurrentToken(), "Expected an identifier");
    }

    auto funIdentifier = getCurrentToken()->getValue();

    // Check Symbol Table
    if (SymbolTable::getInstance().get(funIdentifier))
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

    llvm::Type *returnType;
    if (getCurrentToken()->isDataType())
    {
        returnType = getCurrentToken()->toType(getContext());

        getNextToken(); // eat 'data type'
    }
    else
    {
        returnType = llvm::Type::getVoidTy(*getContext()->getContext());
    }

    auto funcTy = std::make_shared<FunctionType>(returnType, args, isVararg);
    auto func = std::make_shared<Function>(funIdentifier, funcTy);

    // Create Symbol for the function
    {
        SymbolTable::getInstance().insert(funIdentifier, std::make_shared<Attribute>(funIdentifier, AttributeScope::ScopeGlobal, AttributeKind::SymbolFunction, returnType));
    }

    return func;
}
