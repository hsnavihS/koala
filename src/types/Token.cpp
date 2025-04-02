#include <string>

#include "../../include/Token.h"
#include "../../include/TokenType.h"

using namespace std;

Token::Token(TokenType type, string lexeme, string literal, int line) {
  this->type = type;
  this->lexeme = lexeme;
  this->literal = literal;
  this->line = line;
}

string Token::toString() {
  return "Token: " + lexeme + " " + literal + " " + to_string(line);
}
