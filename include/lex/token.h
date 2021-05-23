#pragma once

#include <iostream>
#include <string>
#include "lex/token_kind.h"
#include "ast/type.h"

namespace underrated
{
    class AnalysContext;

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

        void setValue(std::string val) { _value = val; }

        std::string getValue() const { return _value; }
        TokenKind getTokenKind() const { return _kind; }

        bool isKind(TokenKind type) const { return type == _kind; }
        bool isDebug() const { return _kind == TokenKind::TokenDebug; }
        bool isDataType() const { return _kind >= underrated::TokenKind::TokenTyVoid && _kind <= underrated::TokenKind::TokenTyDecimal; }

    public:
        Type *toType(AnalysContext *c, Qualifier qualifier = Qualifier::QualVolatile);
    };
} // namespace underrated

template <typename Enumeration>
auto enumToInt(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}
