#include "lex/lexer.h"

underrated::Token *underrated::Lexer::getPunctuation(char leftChar)
{
    switch (leftChar)
    {
    case '+':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenPuncPlusEqual);
        }
        return createToken(TokenKind::TokenPuncPlus);
    }
    case '-':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenPuncMinusEqual);
        }
        return createToken(TokenKind::TokenPuncMinus);
    }
    case '*':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenPuncStarEqual);
        }
        return createToken(TokenKind::TokenPuncStar);
    }
    case '/':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenPuncSlashEqual);
        }
        return createToken(TokenKind::TokenPuncSlash);
    }
    case '%':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenPuncPercentEqual);
        }
        return createToken(TokenKind::TokenPuncPercent);
    }
    case '^':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenPuncCaretEqual);
        }
        return createToken(TokenKind::TokenPuncCaret);
    }
    case '!':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenPuncNotEqual);
        }
        return createToken(TokenKind::TokenPuncNot);
    }
    case '&':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenPuncAndEqual);
        }
        if (_currentChar == '&')
        {
            getNextChar();

            return createToken(TokenKind::TokenPuncAndAnd);
        }
        return createToken(TokenKind::TokenPuncAnd);
    }
    case '|':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenPuncOrEqual);
        }
        if (_currentChar == '|')
        {
            getNextChar();

            return createToken(TokenKind::TokenPuncOror);
        }
        return createToken(TokenKind::TokenPuncOr);
    }
    case '<':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenPuncLessEqual);
        }
        if (_currentChar == '<')
        {
            getNextChar();
            if (_currentChar == '=')
            {
                getNextChar();

                return createToken(TokenKind::TokenPuncShiftLeftEqual);
            }

            return createToken(TokenKind::TokenPuncShiftLeft);
        }
        return createToken(TokenKind::TokenPuncLessThan);
    }
    case '>':
    {
        if (_currentChar == '=')
        {
            getNextChar();

            return createToken(TokenKind::TokenPuncGreaterEqual);
        }
        if (_currentChar == '>')
        {
            getNextChar();
            if (_currentChar == '=')
            {
                getNextChar();

                return createToken(TokenKind::TokenPuncShiftRightEqual);
            }

            return createToken(TokenKind::TokenPuncShiftRight);
        }
        return createToken(TokenKind::TokenPuncGreaterThen);
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

            return createToken(TokenKind::TokenPuncEqualEqual);
        }
        return createToken(TokenKind::TokenPuncEqual);
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
