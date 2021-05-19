#pragma once

#include <string>
#include "lex/token_kind.h"

namespace underrated
{
    class Token
    {
    private:
        TokenKind _kind;
        std::string _value;
        unsigned _start;
        unsigned _end;

    public:
        Token(TokenKind kind, std::string &value) : _kind(kind), _value(value) {}
        Token(TokenKind kind) : _kind(kind) {}

    public:
        TokenKind getTokenKind() const { return _kind; }
        std::string getValue() const { return _value; }
        void setValue(std::string val) { _value = val; }
    };
} // namespace underrated

template <typename Enumeration>
auto enumToInt(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}
