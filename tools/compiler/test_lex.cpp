#include <iostream>
#include "weasel/lex/lexer.h"

int main()
{
    auto *lexer = new weasel::Lexer();

    while (true)
    {
        std::cout << "Typing > ";
        auto *token = lexer->getNextToken();

        std::cout << enumToInt(token->getTokenKind()) << ":" << token->getValue() << "\n";
    }
}
