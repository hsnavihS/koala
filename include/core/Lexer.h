#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "error/ErrorReporter.h"
#include "types/Token.h"
#include "types/TokenType.h"

using namespace std;

class Lexer {
public:
  Lexer(string code, ErrorReporterPtr errorReporter)
      : code(code), errorReporter(errorReporter) {}
  vector<Token> generateTokens();

private:
  string code;
  int line = 1;
  int column = 0;
  char peek(int i);
  ErrorReporterPtr errorReporter;

  // NOTE: I've completely forgotten how pointers work, maybe
  // revisit this sometime in the future
  Token processString(int *i);
  Token processIdentifier(int *i);
  Token processNumber(int *i);

  unordered_map<string, TokenType> keywords = {
      {"and", TokenType::AND},       {"class", TokenType::CLASS},
      {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
      {"func", TokenType::FUNC},     {"for", TokenType::FOR},
      {"if", TokenType::IF},         {"nil", TokenType::NIL},
      {"or", TokenType::OR},         {"print", TokenType::PRINT},
      {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
      {"this", TokenType::THIS},     {"true", TokenType::TRUE},
      {"var", TokenType::VAR},       {"while", TokenType::WHILE}};
};
