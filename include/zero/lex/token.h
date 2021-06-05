#pragma once

#include <string>

template <typename Enumeration>
auto enumToInt(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

// Type of LLVM
namespace llvm
{
    class Type;
} // namespace llvm

// Base
namespace zero
{
    // Token Kind
    enum class TokenKind
    {
        // End Of File
        TokenEOF,

        // Identifier
        TokenIdentifier,

        // Keyword
        TokenKeyFun,    // fun
        TokenKeyExport, // export
        TokenKeyExtern,
        TokenKeyLet,    // let
        TokenKeyFinal,  // final
        TokenKeyConst,  // const
        TokenKeyReturn, // return
        TokenKeyIf,     // if // Condition
        TokenKeyElse,   // else // Condition
        TokenKeyFor,    // for // Looping

        // Data Literal -> Value of data type
        TokenLitNil,
        TokenLitBool,
        TokenLitNumber,
        TokenLitChar,
        TokenLitString,

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
        TokenOperatorStart,
        TokenOperatorPlus,            // +
        TokenOperatorMinus,           // -
        TokenOperatorStar,            // *
        TokenOperatorSlash,           // /
        TokenOperatorPercent,         // %
        TokenOperatorCaret,           // ^
        TokenOperatorNot,             // !
        TokenOperatorAnd,             // &
        TokenOperatorOr,              // |
        TokenOperatorAndAnd,          // &&
        TokenOperatorOror,            // ||
        TokenOperatorShiftLeft,       // <<
        TokenOperatorShiftRight,      // >>
        TokenOperatorPlusEqual,       // +=
        TokenOperatorMinusEqual,      // -=
        TokenOperatorStarEqual,       // *=
        TokenOperatorSlashEqual,      // /=
        TokenOperatorPercentEqual,    // %=
        TokenOperatorCaretEqual,      // ^=
        TokenOperatorNotEqual,        // !=
        TokenOperatorAndEqual,        // &=
        TokenOperatorOrEqual,         // |=
        TokenOperatorShiftLeftEqual,  // <<=
        TokenOperatorShiftRightEqual, // >>=
        TokenOperatorEqual,           // =
        TokenOperatorEqualEqual,      // ==
        TokenOperatorLessThan,        // <
        TokenOperatorGreaterThen,     // >
        TokenOperatorLessEqual,       // <=
        TokenOperatorGreaterEqual,    // >=
        TokenOperatorEnd,

        TokenPuncDot,       // .
        TokenPuncDotThree,  // ...
        TokenPuncComma,     // ,
        TokenPuncPound,     // #
        TokenPuncQuestion,  // ?
        TokenPuncSemicolon, // ; // optional

        // Punctuation Delimiter Type
        TokenDelimOpenCurlyBracket,   // {
        TokenDelimCloseCurlyBracket,  // }
        TokenDelimOpenSquareBracket,  // [
        TokenDelimCloseSquareBracket, // ]
        TokenDelimOpenParen,          // (
        TokenDelimCloseParen,         // )

        // Space Token
        TokenSpaceNewline, // '\n'

        // Wild card Token
        TokenUndefined,
    };

    // Qualifier for Variable
    enum class Qualifier
    {
        QualConst,
        QualRestrict,
        QualVolatile,
    };

    // Source Location
    struct SourceLocation
    {
        unsigned position = 1;
        unsigned row = 1;
        unsigned col = 1;
        unsigned length = 0;
    };

    // Associativity
    enum class Associative
    {
        LeftToRight,
        RightToLeft,
    };

    // Precendence
    struct Precedence
    {
        Associative associative;
        unsigned order;
    };

    // Default Precedence Order
    static unsigned defPrecOrder = 18;

} // namespace zero

// Token Class
namespace zero
{
    class AnalysContext;

    class Token
    {
    private:
        TokenKind _kind;
        std::string _value;
        SourceLocation _location;

    public:
        Token(TokenKind kind, SourceLocation &location, std::string &value) : _kind(kind), _location(location), _value(value) {}
        Token(TokenKind kind, SourceLocation &location) : _kind(kind), _location(location) {}

        void setValue(std::string val) { _value = val; }
        std::string getValue() const { return _value; }

        SourceLocation getLocation() const { return _location; }
        TokenKind getTokenKind() const { return _kind; }
        int getTokenKindToInt() { return enumToInt(_kind); }

        bool isKind(TokenKind type) const { return type == _kind; }
        bool isDataType() const { return _kind >= TokenKind::TokenTyVoid && _kind <= TokenKind::TokenTyDecimal; }
        bool isKeyDefinition() const { return (_kind == TokenKind::TokenKeyLet || _kind == TokenKind::TokenKeyFinal || _kind == TokenKind::TokenKeyConst); }
        bool isLiteral() const { return _kind >= TokenKind::TokenLitNil && _kind <= TokenKind::TokenLitString; }
        bool isOperator() const { return _kind >= TokenKind::TokenOperatorStart && _kind <= TokenKind::TokenOperatorEnd; }
        bool isNewline() const { return _kind == TokenKind::TokenSpaceNewline; }

        Qualifier getQualifier();

        // TODO: Need to support Right to Left Associativity
        Precedence getPrecedence();

    public:
        llvm::Type *toType(AnalysContext *c);
    };
} // namespace zero
