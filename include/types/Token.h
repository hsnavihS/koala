#pragma once

#include <any>
#include <string>

#include "TokenType.h"

using namespace std;

class Token {
public:
  Token(TokenType type, string lexeme, any literal, int line, int column)
      : type(type), lexeme(lexeme), literal(literal), line(line),
        column(column) {}

  string toString() {
    return "Lexeme: " + lexeme + ", {" +
           "Type: " + tokenTypeToString[static_cast<int>(type)] + ", " +
           "Line: " + to_string(line) + ", " + "Column: " + to_string(column) +
           "}";
  }

  // Getters
  string getLexeme() { return lexeme; }
  TokenType getType() { return type; }
  any getLiteral() { return literal; }
  int getLine() { return line; }
  int getColumn() { return column; }

private:
  string lexeme;
  TokenType type;
  any literal;
  int line;
  int column;
};
