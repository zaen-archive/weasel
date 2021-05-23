// #include "parse/parser.h"

// underrated::Expression *underrated::Parser::parseScalarExpr()
// {
//     return new LiteralExpr(std::stod(this->getCurrentToken()->getValue()));
// }

// underrated::Expression *underrated::Parser::parseExpr()
// {
//     auto *lhs = this->getCurrentToken();
//     if (lhs->getTokenKind() == TokenKind::TokenTyInt)
//     {
//         return parseScalarExpr();
//     }

//     return nullptr;
// }

// underrated::Expression *underrated::Parser::parseVariableExpr()
// {
//     // Eat 'let'
//     auto *token = getNextToken();
//     if (token->getTokenKind() != TokenKind::TokenIdentifier)
//     {
//         return logError("Expecting identifier");
//     }

//     // Eat 'identifier'
//     auto identifier = token->getValue();
//     token = getNextToken();
//     if (token->getTokenKind() == TokenKind::TokenTyInt)
//     {
//         // Our data type just integer
//         // TODO: Just get next token
//         token = getNextToken();

//         // If not define the value yet
//         if (token->getTokenKind() == TokenKind::TokenPuncSemicolon)
//         {
//             return new VariableExpr(identifier, new LiteralExpr(0));
//         }
//     }

//     // Should get assign operator
//     if (token->getTokenKind() != TokenKind::TokenPuncEqual)
//     {
//         return logError("Expecting assign operator");
//     }

//     // Eat 'Assign' Operator
//     getNextToken();

//     // Get An Expression
//     auto *expr = parseExpr();

//     if (!expr)
//     {
//         return logError("Expecting an expression");
//     }

//     if (getNextToken()->getTokenKind() != TokenKind::TokenPuncSemicolon)
//     {
//         return logError("Expecting new line or semicolon");
//     }

//     return new VariableExpr(identifier, expr);
// }
