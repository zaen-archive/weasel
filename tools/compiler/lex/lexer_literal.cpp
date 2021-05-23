#include "lex/lexer.h"

underrated::Token *underrated::Lexer::getStringLiteral()
{
    std::string text = "";

    while (true)
    {
        if (_lastChar == '\n')
        {
            return nullptr;
        }

        if (_lastChar == '"')
        {
            if (text.back() == '\\')
            {
                text[text.size() - 1] = _lastChar;
            }
            else
            {
                break;
            }
        }

        text += _lastChar;

        getNextChar();
    }

    getNextToken(); // eat '"'

    return new Token(TokenKind::TokenLitString, text);
}

underrated::Token *underrated::Lexer::getCharacterLiteral()
{
    auto c = _lastChar;
    if (getNextChar() != '\'')
    {
        return nullptr;
    }
    getNextChar(); // eat '''

    return new Token(TokenKind::TokenLitChar, std::string(1, c));
}
