#include "zero/parse/parser.h"
#include "zero/symbol/symbol.h"

void zero::Parser::ignoreNewline()
{
    if (getCurrentToken()->isNewline())
    {
        getNextToken(true);
    }
}

llvm::Type *zero::Parser::parseDataType()
{
    auto isPointerType = false;
    if (getCurrentToken()->isKind(TokenKind::TokenOperatorStar))
    {
        getNextToken();
        isPointerType = true;
    }

    if (getCurrentToken()->isDataType())
    {
        auto *type = getCurrentToken()->toType(getContext(), isPointerType);

        getNextToken();
        return type;
    }

    if (isPointerType)
    {
        return ErrorTable::addError(getCurrentToken(), "Expected data type after pointer type");
    }

    return nullptr;
}
