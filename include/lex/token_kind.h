#pragma once

namespace underrated
{
    // TOKEN KIND
    enum class TokenKind
    {
        // Undefined
        TokenEOF,
        TokenUndefined,

        // Keyword
        TokenKeyFun,
        TokenKeyExtern,
        TokenKeyLet,
        TokenKeyFinal,
        TokenKeyInt,

        // Primary
        TokenIdentifier,
        TokenNumber,

        // Parentise
        TokenOpenParen,
        TokenCloseParen,

        // Comma
        TokenComma,

        // Operator
        TokenOpPlus,
        TokenOpMinus,
        TokenOpMul,
        TokenOpDivide,
        TokenOpMod,
        TokenOpLessThan,
        TokenOpLessEqual,
        TokenOpGreaterThen,
        TokenOpGreaterEqual,
        TokenOpAssign,

        // Top Level Semicolons
        TokenSemicolon,

        // Debug
        TokenDebug,
    };
} // namespace underrated
