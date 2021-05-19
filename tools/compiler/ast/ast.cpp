#include "ast/ast.h"

underrated::ExprAST *underrated::logError(const char *msg)
{
    fprintf(stderr, "LogError: %s\n", msg);
    return nullptr;
}
