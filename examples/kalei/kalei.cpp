#include <string>
#include <iostream>

// LEXER
enum TokenKind {
    eof,
    key_fun,
    identifier,
    number
};

static std::string _identifierStr;
static double _numVal;

/// Declare Function ///
// LEXER
void logError(std::string msg);
static int getTokenKind();

/// Define Function ///
// LEXER //
void logError(std::string msg) {
    std::cout << "Error : " << msg << std::endl;
    exit(2);
}

static int getTokenKind() {
    static int lastChar = ' ';

    while (std::isspace(lastChar))
    {
        lastChar = std::getchar();
    }
    
    // Alphanum character [A-Za-z][a-zA-Z0-9]
    while (std::isalpha(lastChar))
    {
        _identifierStr = lastChar;
        while (std::isalnum(lastChar = std::getchar()))
        {
            _identifierStr += lastChar;
        }
        
        if (_identifierStr == "fun") {
            return TokenKind::key_fun;
        }
    }
    
    // Digit Character [0-9]
    while (std::isdigit(lastChar))
    {
        std::string numStr;

        do {
            numStr += lastChar;
            lastChar = std::getchar();
        } while (std::isdigit(lastChar) || lastChar == '.');

        _numVal = std::strtod(numStr.c_str(), 0);
        return TokenKind::number;
    }

    // Comment Character //
    if (lastChar == '/') {
        if ((lastChar = getchar()) == '/') {
            logError("Expecting /, but get " + lastChar);
        }
        
        do {
            lastChar = getchar();
        } while (lastChar != EOF || lastChar != '\n' || lastChar != '\r');
        
        if (lastChar != EOF) {
            return getTokenKind();
        }
    }

    return lastChar;    
}


// PARSER //


// Main Loop
void mainLoop() {
    while (true)
    {
        printf("ready > ");

    }
    
}

int main() {
}
