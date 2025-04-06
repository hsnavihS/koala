#include <string>

#include <any>

#include "core/Token.h"
#include "core/TokenType.h"

using namespace std;

Token::Token(TokenType type, string lexeme, any literal, int line) {
  this->type = type;
  this->lexeme = lexeme;
  this->literal = literal;
  this->line = line;
}

string Token::toString() {
  return "Lexeme: " + lexeme + ", {" +
         "Type: " + tokenTypeToString[static_cast<int>(type)] + ", " +
         "Line: " + to_string(line) + "}";
}
