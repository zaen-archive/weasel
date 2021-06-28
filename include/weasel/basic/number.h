#pragma once

#include <string>

namespace weasel
{
    class Number
    {
    public:
        static bool isInteger(std::string val);
        static bool isFloat(std::string val);
        static long long toInteger(std::string val);
    };

} // namespace weasel
