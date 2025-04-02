#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Token.h"
#include "TokenType.h"

using namespace std;

class Parser {
public:
  Parser(string code) : code(code) {}
  vector<Token> parseTokens();

private:
  string code;
  int line = 1;
  char peek(int i);
  // NOTE: I've completely forgotten how pointers work, maybe
  // revisit this sometime in the future
  Token parseString(int *i);
  Token parseIdentifier(int *i);
  Token parseNumber(int *i);
  unordered_map<string, TokenType> keywords = {
    {"and", TokenType::AND},
    {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},
    {"false", TokenType::FALSE},
    {"func", TokenType::FUNC},
    {"for", TokenType::FOR},
    {"if", TokenType::IF},
    {"nil", TokenType::NIL},
    {"or", TokenType::OR},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super", TokenType::SUPER},
    {"this", TokenType::THIS},
    {"true", TokenType::TRUE},
    {"var", TokenType::VAR},
    {"while", TokenType::WHILE}
  };
};
