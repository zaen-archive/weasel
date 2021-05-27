#include "parse/parser.h"
#include "analysis/context.h"

underrated::LiteralExpression *underrated::Parser::parseLiteralExpression()
{
    auto *token = getCurrentToken();
    auto value = strtoll(token->getValue().c_str(), 0, 10);

    getNextToken(); // eat 'literal'

    return new NumberLiteralExpression(value);
}
