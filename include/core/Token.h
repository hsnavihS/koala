#pragma once

#include <any>
#include <string>

#include "TokenType.h"

using namespace std;

class Token {
public:
  Token(TokenType type, string lexeme, any literal, int line);
  string toString();
  string getLexeme() { return lexeme; }

  TokenType getType() { return type; }
  any getLiteral() { return literal; }
  int getLine() { return line; }

private:
  string lexeme;
  TokenType type;
  any literal;
  int line;
};
