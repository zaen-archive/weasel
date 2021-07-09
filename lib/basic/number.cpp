#include "weasel/basic/number.h"

long long weasel::Number::toInteger(std::string val)
{
    return std::stoll(val.c_str(), 0, 10);
}

bool weasel::Number::isInteger(std::string val)
{
    for (size_t i = 0; i < val.size(); i++)
    {
        if (!isdigit(val[i])) return false;
    }

    return true;
}

bool weasel::Number::isFloat(std::string val)
{
    auto dot = false;
    for (size_t i = 0; i < val.size(); i++)
    {
        auto c = val[i];
        if (c == '.') {
            if (dot) {
                return false;
            }
            dot = true;
        } else if (!isdigit(val[i])) {
            return false;
        }
    }

    return true;
}
