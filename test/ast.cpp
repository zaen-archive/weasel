#include "ast.h"

namespace underrated
{
    std::unique_ptr<ExprAST> logError(const char *Str)
    {
        fprintf(stderr, "LogError: %s\n", Str);
        return nullptr;
    }

    std::unique_ptr<ProtorypeAST> logErrorP(const char *Str)
    {
        logError(Str);
        return nullptr;
    }
} // namespace underrated
