#include <llvm/IR/DerivedTypes.h>
#include "weasel/basic/number.h"
#include "weasel/parse/parser.h"
#include "weasel/symbol/symbol.h"

void weasel::Parser::ignoreNewline()
{
    if (getCurrentToken()->isNewline())
    {
        getNextToken(true);
    }
}

std::vector<std::shared_ptr<weasel::Function>> weasel::Parser::getParallelFunctions()
{
    std::vector<std::shared_ptr<Function>> funs(_parallelCount);
    unsigned idx = 0;

    for (const auto &item : _funs) {
        if (item->getParallelType() != ParallelType::None) {
            funs[idx++] = item;
        }
    }

    return funs;
}

llvm::Type *weasel::Parser::parseDataType()
{
    // Pointer
    if (getCurrentToken()->isKind(TokenKind::TokenOperatorStar))
    {
        if (!getNextToken()->isDataType())
        {
            return ErrorTable::addError(getCurrentToken(), "Expected data type after pointer type");
        }

        auto *type = getCurrentToken()->toType(*getContext(), true);

        getNextToken();
        return type;
    }

    // Array
    if (getCurrentToken()->isKind(TokenKind::TokenDelimOpenSquareBracket))
    {
        if (!getNextToken()->isKind(TokenKind::TokenLitNumber))
        {
            return ErrorTable::addError(getCurrentToken(), "Expected size of array");
        }

        auto numStr = getCurrentToken()->getValue();
        if (!weasel::Number::isInteger(numStr))
        {
            return ErrorTable::addError(getCurrentToken(), "Number is not a valid integer");
        }

        if (!getNextToken()->isKind(TokenKind::TokenDelimCloseSquareBracket))
        {
            return ErrorTable::addError(getCurrentToken(), "Expected ] for array type");
        }

        if (!getNextToken()->isDataType())
        {
            return ErrorTable::addError(getCurrentToken(), "Expected data type after [...]");
        }

        auto num = weasel::Number::toInteger(numStr);
        auto *type = getCurrentToken()->toType(*getContext());
        auto *arrTy = llvm::ArrayType::get(type, num);

        getNextToken();

        return arrTy;
    }

    // Normal Data Type or no datatype
    auto *type = getCurrentToken()->toType(*getContext());
    if (type == nullptr)
    {
        return nullptr;
    }
    getNextToken();

    return type;
}
