#include "zero/lex/lexer.h"

zero::Token *zero::Lexer::getPunctuation(char leftChar)
{
    switch (leftChar)
    {
    case '+':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorPlusEqual);
        }
        return createToken(TokenKind::TokenOperatorPlus);
    }
    case '-':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorMinusEqual);
        }
        return createToken(TokenKind::TokenOperatorMinus);
    }
    case '*':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorStarEqual);
        }
        return createToken(TokenKind::TokenOperatorStar);
    }
    case '/':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorSlashEqual);
        }
        return createToken(TokenKind::TokenOperatorSlash);
    }
    case '%':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorPercentEqual);
        }
        return createToken(TokenKind::TokenOperatorPercent);
    }
    case '^':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorCaretEqual);
        }
        return createToken(TokenKind::TokenOperatorCaret);
    }
    case '!':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorNotEqual);
        }
        return createToken(TokenKind::TokenOperatorNot);
    }
    case '&':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorAndEqual);
        }
        if (_currentChar == '&')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorAndAnd);
        }
        return createToken(TokenKind::TokenOperatorAnd);
    }
    case '|':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorOrEqual);
        }
        if (_currentChar == '|')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorOror);
        }
        return createToken(TokenKind::TokenOperatorOr);
    }
    case '<':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorLessEqual);
        }
        if (_currentChar == '<')
        {
            getNextChar();
            if (_currentChar == '=')
            {
                getNextChar();

                return createToken(TokenKind::TokenOperatorShiftLeftEqual);
            }

            return createToken(TokenKind::TokenOperatorShiftLeft);
        }
        return createToken(TokenKind::TokenOperatorLessThan);
    }
    case '>':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorGreaterEqual);
        }
        if (_currentChar == '>')
        {
            getNextChar();
            if (_currentChar == '=')
            {
                getNextChar();

                return createToken(TokenKind::TokenOperatorShiftRightEqual);
            }

            return createToken(TokenKind::TokenOperatorShiftRight);
        }
        return createToken(TokenKind::TokenOperatorGreaterThen);
    }
    case '.':
    {
        if (_currentChar == '.')
        {
            if (getNextChar() == '.')
            {
                getNextChar();

                return createToken(TokenKind::TokenPuncDotThree);
            }

            return nullptr;
        }

        return createToken(TokenKind::TokenPuncDot);
    }
    case '#':
    {
        return createToken(TokenKind::TokenPuncPound);
    }
    case '?':
    {
        return createToken(TokenKind::TokenPuncQuestion);
    }
    case '=':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenOperatorEqualEqual);
        }
        return createToken(TokenKind::TokenOperatorEqual);
    }
    case ',':
    {
        return createToken(TokenKind::TokenPuncComma);
    }
    case '(':
    {
        return createToken(TokenKind::TokenDelimOpenParen);
    }
    case ')':
    {
        return createToken(TokenKind::TokenDelimCloseParen);
    }
    case '[':
    {
        return createToken(TokenKind::TokenDelimOpenSquareBracket);
    }
    case ']':
    {
        return createToken(TokenKind::TokenDelimCloseSquareBracket);
    }
    case '{':
    {
        return createToken(TokenKind::TokenDelimOpenCurlyBracket);
    }
    case '}':
    {
        return createToken(TokenKind::TokenDelimCloseCurlyBracket);
    }
    case ';':
    {
        return createToken(TokenKind::TokenPuncSemicolon);
    }

    default:
        return nullptr;
    }
}
