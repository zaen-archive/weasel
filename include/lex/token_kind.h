#pragma once

namespace underrated
{
    // TOKEN KIND
    enum class TokenKind
    {
        // End Of File
        TokenEOF,

        // Identifier
        TokenIdentifier,

        // Keyword
        TokenKeyFun,    // fun
        TokenKeyExport, // export
        TokenKeyLet,    // let
        TokenKeyFinal,  // final
        TokenKeyConst,  // const
        TokenKeyReturn, // return
        TokenKeyIf,     // if // Condition
        TokenKeyElse,   // else // Condition
        TokenKeyFor,    // for // Looping

        // Data Literal -> Value of data type
        TokenLitString,
        TokenLitChar,
        TokenLitNumber,
        TokenLitBool,

        /// Data Types -> literally name of the data type
        TokenTyVoid,    // Void No Return Value
        TokenTyRune,    // 4 byte Unicode // Character 'utf-32'
        TokenTyString,  // Character 'utf-8' with 'utf-32' support
        TokenTyByte,    // 1 byte // Integer Character 'utf-8'
        TokenTySbyte,   // 1 byte // Integer
        TokenTyShort,   // 2 byte // Integer
        TokenTyUshort,  // 2 byte // Integer
        TokenTyInt,     // 4 byte // Integer
        TokenTyUint,    // 4 byte // Integer
        TokenTyLong,    // 8 byte // Integer
        TokenTyUlong,   // 8 byte // Integer
        TokenTyInt128,  // 16 byte // Integer
        TokenTyUnt128,  // 16 byte // Integer // NOT USED
        TokenTyBool,    // 1 byte // Boolean
        TokenTyFloat,   // 4 byte // Floating Point
        TokenTyDouble,  // 8 byte // Floating Point
        TokenTyDecimal, // 16 byte // Floating Point

        // Punctuation
        TokenPuncPlus,            // +
        TokenPuncMinus,           // -
        TokenPuncStar,            // *
        TokenPuncSlash,           // /
        TokenPuncPercent,         // %
        TokenPuncCaret,           // ^
        TokenPuncNot,             // !
        TokenPuncAnd,             // &
        TokenPuncOr,              // |
        TokenPuncAndAnd,          // &&
        TokenPuncOror,            // ||
        TokenPuncShiftLeft,       // <<
        TokenPuncShiftRight,      // >>
        TokenPuncPlusEqual,       // +=
        TokenPuncMinusEqual,      // -=
        TokenPuncStarEqual,       // *=
        TokenPuncSlashEqual,      // /=
        TokenPuncPercentEqual,    // %=
        TokenPuncCaretEqual,      // ^=
        TokenPuncNotEqual,        // !=
        TokenPuncAndEqual,        // &=
        TokenPuncOrEqual,         // |=
        TokenPuncShiftLeftEqual,  // <<=
        TokenPuncShiftRightEqual, // >>=
        TokenPuncEqual,           // =
        TokenPuncEqualEqual,      // ==
        TokenPuncLessThan,        // <
        TokenPuncGreaterThen,     // >
        TokenPuncLessEqual,       // <=
        TokenPuncGreaterEqual,    // >=
        TokenPuncDot,             // .
        TokenPuncDotThree,        // ...
        TokenPuncComma,           // ,
        TokenPuncPound,           // #
        TokenPuncQuestion,        // ?
        TokenPuncSemicolon,       // ; // optional

        // Punctuation Delimiter Type
        TokenDelimOpenCurlyBracket,   // {
        TokenDelimCloseCurlyBracket,  // }
        TokenDelimOpenSquareBracket,  // [
        TokenDelimCloseSquareBracket, // ]
        TokenDelimOpenParen,          // (
        TokenDelimCloseParen,         // )

        // Space Token
        TokenSpaceNewline, // '\n'

        // Debug
        TokenDebug,
        TokenUndefined,
    };
} // namespace underrated
