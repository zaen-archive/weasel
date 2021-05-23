#include "lex/lexer.h"

underrated::Token *underrated::Lexer::getPunctuation(char leftChar)
{
    switch (leftChar)
    {
    case '+':
    {
        if (_lastChar == '=')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncPlusEqual);
        }
        return new Token(TokenKind::TokenPuncPlus);
    }
    case '-':
    {
        if (_lastChar == '=')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncMinusEqual);
        }
        return new Token(TokenKind::TokenPuncMinus);
    }
    case '*':
    {
        if (_lastChar == '=')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncStarEqual);
        }
        return new Token(TokenKind::TokenPuncStar);
    }
    case '/':
    {
        if (_lastChar == '=')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncSlashEqual);
        }
        return new Token(TokenKind::TokenPuncSlash);
    }
    case '%':
    {
        if (_lastChar == '=')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncPercentEqual);
        }
        return new Token(TokenKind::TokenPuncPercent);
    }
    case '^':
    {
        if (_lastChar == '=')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncCaretEqual);
        }
        return new Token(TokenKind::TokenPuncCaret);
    }
    case '!':
    {
        if (_lastChar == '=')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncNotEqual);
        }
        return new Token(TokenKind::TokenPuncNot);
    }
    case '&':
    {
        if (_lastChar == '=')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncAndEqual);
        }
        if (_lastChar == '&')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncAndAnd);
        }
        return new Token(TokenKind::TokenPuncAnd);
    }
    case '|':
    {
        if (_lastChar == '=')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncOrEqual);
        }
        if (_lastChar == '|')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncOror);
        }
        return new Token(TokenKind::TokenPuncOr);
    }
    case '<':
    {
        if (_lastChar == '=')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncLessEqual);
        }
        if (_lastChar == '<')
        {
            getNextChar();
            if (_lastChar == '=')
            {
                getNextChar();

                return new Token(TokenKind::TokenPuncShiftLeftEqual);
            }

            return new Token(TokenKind::TokenPuncShiftLeft);
        }
        return new Token(TokenKind::TokenPuncLessThan);
    }
    case '>':
    {
        if (_lastChar == '=')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncGreaterEqual);
        }
        if (_lastChar == '>')
        {
            getNextChar();
            if (_lastChar == '=')
            {
                getNextChar();

                return new Token(TokenKind::TokenPuncShiftRightEqual);
            }

            return new Token(TokenKind::TokenPuncShiftRight);
        }
        return new Token(TokenKind::TokenPuncGreaterThen);
    }
    case '.':
    {
        if (_lastChar == '.')
        {
            if (getNextChar() == '.')
            {
                getNextChar();

                return new Token(TokenKind::TokenPuncDotThree);
            }

            return nullptr;
        }

        return new Token(TokenKind::TokenPuncDot);
    }
    case '#':
    {
        return new Token(TokenKind::TokenPuncPound);
    }
    case '?':
    {
        return new Token(TokenKind::TokenPuncQuestion);
    }
    case '=':
    {
        if (_lastChar == '=')
        {
            getNextChar();

            return new Token(TokenKind::TokenPuncEqualEqual);
        }
        return new Token(TokenKind::TokenPuncEqual);
    }
    case ',':
    {
        return new Token(TokenKind::TokenPuncComma);
    }
    case '(':
    {
        return new Token(TokenKind::TokenDelimOpenParen);
    }
    case ')':
    {
        return new Token(TokenKind::TokenDelimCloseParen);
    }
    case '[':
    {
        return new Token(TokenKind::TokenDelimOpenSquareBracket);
    }
    case ']':
    {
        return new Token(TokenKind::TokenDelimCloseSquareBracket);
    }
    case '{':
    {
        return new Token(TokenKind::TokenDelimOpenCurlyBracket);
    }
    case '}':
    {
        return new Token(TokenKind::TokenDelimCloseCurlyBracket);
    }
    case ';':
    {
        return new Token(TokenKind::TokenPuncSemicolon);
    }

    default:
        return new Token(TokenKind::TokenUndefined, std::string(1, leftChar));
    }
}
