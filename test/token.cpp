#include "token.h"

namespace underrated
{
    std::string _identifier_ = "";
    char _lastChar_ = ' ';
    char _undefined_ = ' ';
    double _numValue_ = 0;
    TokenKind _currentToken_;

    TokenKind getToken()
    {
        // Remove Space
        while (isspace(_lastChar_))
        {
            _lastChar_ = getchar();
        }

        // Check identifier
        if (isalpha(_lastChar_))
        {
            _identifier_ = "";
            do
            {
                _identifier_ += _lastChar_;
            } while (isalnum(_lastChar_ = getchar()));

            if (_identifier_ == "fun")
            {
                return TokenKind::TokenFun;
            }

            if (_identifier_ == "extern")
            {
                return TokenKind::TokenExtern;
            }

            return TokenKind::TokenIdentifier;
        }

        // Check Number
        if (isdigit(_lastChar_) || _lastChar_ == '.')
        {
            std::string numStr;
            do
            {
                numStr += _lastChar_;
                _lastChar_ = getchar();
            } while (isdigit(_lastChar_) || _lastChar_ == '.');

            _numValue_ = strtod(numStr.c_str(), 0);
            return TokenKind::TokenNumber;
        }

        // Check Comment
        if (_lastChar_ == '#')
        {
            do
            {
                _lastChar_ = getchar();
            } while (_lastChar_ != EOF && _lastChar_ != '\n' && _lastChar_ != '\r');

            if (_lastChar_ != EOF)
            {
                return getToken();
            }
        }

        // Check if end of FIle
        if (_lastChar_ == EOF)
        {
            return TokenKind::TokenEOF;
        }

        // Save Current Character
        _undefined_ = _lastChar_;
        _lastChar_ = getchar();

        // Parentise
        if (_lastChar_ == '(')
        {
            return TokenKind::TokenOpenParen;
        }
        if (_lastChar_ == ')')
        {
            return TokenKind::TokenCloseParen;
        }

        // Comma
        if (_lastChar_ == ',')
        {
            return TokenKind::TokenComma;
        }

        // Semi Colon
        if (_lastChar_ == ';')
        {
            return TokenKind::TokenSemicolon;
        }

        // Not Spesified
        return TokenKind::TokenUndefined;
    }

    TokenKind getNextToken()
    {
        return _currentToken_ = getToken();
    }

    int getTokenPrecedence()
    {
        if (!isascii(_currentToken_))
        {
            return -1;
        }

        int prec = _binopPrecedence_[_currentToken_];
        if (prec <= 0)
        {
            return -1;
        }

        return prec;
    }

} // namespace underrated
