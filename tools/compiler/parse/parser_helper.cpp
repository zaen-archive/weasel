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

llvm::Type *weasel::Parser::parseDataType()
{
    if (getCurrentToken()->isKind(TokenKind::TokenOperatorStar)) {
        if (!getNextToken()->isDataType())
        {
            return ErrorTable::addError(getCurrentToken(), "Expected data type after pointer type");
        }
        
        auto *type = getCurrentToken()->toType(getContext(), true);

        getNextToken();
        return type;
    }

    if (getCurrentToken()->isKind(TokenKind::TokenDelimOpenSquareBracket)) {
        if (!getNextToken()->isKind(TokenKind::TokenLitNumber))
        {
            return ErrorTable::addError(getCurrentToken(), "Expected size of array");
        }

        auto numStr = getCurrentToken()->getValue();
        if (!weasel::Number::isInteger(numStr)) {
            return ErrorTable::addError(getCurrentToken(), "Number is not a valid integer");
        }

        if (!getNextToken()->isKind(TokenKind::TokenDelimCloseSquareBracket))
        {
            return ErrorTable::addError(getCurrentToken(), "Expected ] for array type");
        }

        if (!getNextToken()->isDataType()) {
            return ErrorTable::addError(getCurrentToken(), "Expected data type after [...]");
        }

        auto num = weasel::Number::toInteger(numStr);
        auto *type = getCurrentToken()->toType(getContext());
        auto *arrTy = llvm::ArrayType::get(type, num);

        getNextToken();

        return arrTy;
    }

    return nullptr;
}
