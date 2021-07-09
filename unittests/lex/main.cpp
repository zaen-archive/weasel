#include <iostream>
#include <memory>
#include "weasel/lex/lexer.h"

int main(int argc, char *argv[])
{
    auto *fileManager = new weasel::FileManager("runtime-rt/test/main.we");
    if (!fileManager->isValid())
    {
        std::cout << "File Not exist\n";
        return 0;
    }

    auto lexer = std::make_unique<weasel::Lexer>(fileManager);

    while (!lexer->getNextToken(true)->isKind(weasel::TokenKind::TokenEOF))
    {
        auto token = lexer->getCurrentToken();

        std::cout << enumToInt(token->getTokenKind()) << ":" << token->getValue() << "\n";
    }
}
