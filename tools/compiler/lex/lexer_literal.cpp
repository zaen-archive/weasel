#include "zero/lex/lexer.h"

zero::Token *zero::Lexer::getStringLiteral()
{
    std::string text = "";

    while (true)
    {
        if (_currentChar == '\n')
        {
            return nullptr;
        }

        if (_currentChar == '"')
        {
            if (text.back() == '\\')
            {
                text[text.size() - 1] = _currentChar;
            }
            else
            {
                break;
            }
        }

        text += _currentChar;

        getNextChar();
    }

    getNextToken(); // eat '"'

    return createToken(TokenKind::TokenLitString, text);
}

zero::Token *zero::Lexer::getCharacterLiteral()
{
    auto c = _currentChar;
    if (getNextChar() != '\'')
    {
        return nullptr;
    }
    getNextChar(); // eat '''

    return createToken(TokenKind::TokenLitChar, std::string(1, c));
}
