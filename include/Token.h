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

private:
  string lexeme;
  TokenType type;
  any literal;
  int line;
};
