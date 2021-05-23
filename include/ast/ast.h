#pragma once

#include <iostream>
#include "ast/expr.h"
#include "ast/function.h"

namespace underrated
{
    Expression *logError(const char *msg);
    Expression *logErrorV(const char *msg);
    Func *logErrorF(const char *msg);
} // namespace underrated
