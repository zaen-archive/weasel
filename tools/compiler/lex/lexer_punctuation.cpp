#include "weasel/lex/lexer.h"

std::shared_ptr<weasel::Token> weasel::Lexer::getPunctuation()
{
    auto *lastBuffer = _currentBuffer - 1;

    switch (*lastBuffer)
    {
    case '+':
    {
        if (*(lastBuffer + 1) == '=')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorPlusEqual, lastBuffer, lastBuffer + 2);
        }

        return createToken(TokenKind::TokenOperatorPlus, lastBuffer, lastBuffer + 1);
    }
    case '-':
    {
        if (*(lastBuffer + 1) == '=')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorMinusEqual, lastBuffer, lastBuffer + 2);
        }

        return createToken(TokenKind::TokenOperatorMinus, lastBuffer, lastBuffer + 1);
    }
    case '*':
    {
        if (*(lastBuffer + 1) == '=')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorStarEqual, lastBuffer, lastBuffer + 2);
        }

        return createToken(TokenKind::TokenOperatorStar, lastBuffer, lastBuffer + 1);
    }
    case '/':
    {
        if (*(lastBuffer + 1) == '=')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorSlashEqual, lastBuffer, lastBuffer + 2);
        }

        return createToken(TokenKind::TokenOperatorSlash, lastBuffer, lastBuffer + 1);
    }
    case '%':
    {
        if (*(lastBuffer + 1) == '=')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorPercentEqual, lastBuffer, lastBuffer + 2);
        }

        return createToken(TokenKind::TokenOperatorPercent, lastBuffer, lastBuffer + 1);
    }
    case '^':
    {
        if (*(lastBuffer + 1) == '=')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorCaretEqual, lastBuffer, lastBuffer + 2);
        }

        return createToken(TokenKind::TokenOperatorCaret, lastBuffer, lastBuffer + 1);
    }
    case '!':
    {
        if (*(lastBuffer + 1) == '=')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorNotEqual, lastBuffer, lastBuffer + 2);
        }

        return createToken(TokenKind::TokenOperatorNot, lastBuffer, lastBuffer + 1);
    }
    case '&':
    {
        if (*(lastBuffer + 1) == '=')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorAndEqual, lastBuffer, lastBuffer + 2);
        }
        if (*(lastBuffer + 1) == '&')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorAndAnd, lastBuffer, lastBuffer + 2);
        }

        return createToken(TokenKind::TokenOperatorAnd, lastBuffer, lastBuffer + 1);
    }
    case '|':
    {
        if (*(lastBuffer + 1) == '=')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorOrEqual, lastBuffer, lastBuffer + 2);
        }
        if (*(lastBuffer + 1) == '|')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorOror, lastBuffer, lastBuffer + 2);
        }

        return createToken(TokenKind::TokenOperatorOr, lastBuffer, lastBuffer + 1);
    }
    case '<':
    {
        if (*(lastBuffer + 1) == '=')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorLessEqual, lastBuffer, lastBuffer + 2);
        }

        if (*(lastBuffer + 1) == '<')
        {
            if (*(lastBuffer + 2) == '=')
            {
                getNextBuffer(2);
                return createToken(TokenKind::TokenOperatorShiftLeftEqual, lastBuffer, lastBuffer + 3);
            }

            getNextBuffer();
            return createToken(TokenKind::TokenOperatorShiftLeft, lastBuffer, lastBuffer + 2);
        }

        return createToken(TokenKind::TokenOperatorLessThan, lastBuffer, lastBuffer + 1);
    }
    case '>':
    {
        if (*(lastBuffer + 1) == '=')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorGreaterEqual, lastBuffer, lastBuffer + 2);
        }

        if (*(lastBuffer + 1) == '>')
        {
            if (*(lastBuffer + 2) == '=')
            {
                getNextBuffer(2);
                return createToken(TokenKind::TokenOperatorShiftRightEqual, lastBuffer, lastBuffer + 3);
            }

            getNextBuffer();
            return createToken(TokenKind::TokenOperatorShiftRight, lastBuffer, lastBuffer + 2);
        }

        return createToken(TokenKind::TokenOperatorGreaterThen, lastBuffer, lastBuffer + 1);
    }
    case '.':
    {
        if (*(lastBuffer + 1) == '.')
        {
            if (*(lastBuffer + 2) == '.')
            {
                getNextBuffer(2);
                return createToken(TokenKind::TokenPuncDotThree, lastBuffer, lastBuffer + 3);
            }

            return nullptr;
        }

        return createToken(TokenKind::TokenPuncDot, lastBuffer, lastBuffer + 1);
    }
    case '#':
    {
        return createToken(TokenKind::TokenPuncPound, lastBuffer, lastBuffer + 1);
    }
    case '?':
    {
        return createToken(TokenKind::TokenPuncQuestion, lastBuffer, lastBuffer + 1);
    }
    case '=':
    {
        if (*(lastBuffer + 1) == '=')
        {
            getNextBuffer();
            return createToken(TokenKind::TokenOperatorEqualEqual, lastBuffer, lastBuffer + 2);
        }

        return createToken(TokenKind::TokenOperatorEqual, lastBuffer, lastBuffer + 1);
    }
    case ',':
    {
        return createToken(TokenKind::TokenPuncComma, lastBuffer, lastBuffer + 1);
    }
    case '(':
    {
        return createToken(TokenKind::TokenDelimOpenParen, lastBuffer, lastBuffer + 1);
    }
    case ')':
    {
        return createToken(TokenKind::TokenDelimCloseParen, lastBuffer, lastBuffer + 1);
    }
    case '[':
    {
        return createToken(TokenKind::TokenDelimOpenSquareBracket, lastBuffer, lastBuffer + 1);
    }
    case ']':
    {
        return createToken(TokenKind::TokenDelimCloseSquareBracket, lastBuffer, lastBuffer + 1);
    }
    case '{':
    {
        return createToken(TokenKind::TokenDelimOpenCurlyBracket, lastBuffer, lastBuffer + 1);
    }
    case '}':
    {
        return createToken(TokenKind::TokenDelimCloseCurlyBracket, lastBuffer, lastBuffer + 1);
    }
    case ';':
    {
        return createToken(TokenKind::TokenPuncSemicolon, lastBuffer, lastBuffer + 1);
    }

    default:
        return nullptr;
    }
}
