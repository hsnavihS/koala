#pragma once

#include <string>
#include <vector>

#include "Token.h"

using namespace std;

class Parser {
public:
  Parser();
  void runPrompt();
  void runFile(char *filename);
  void reportError(int line, string where, string message);

private:
  bool hadError = false;
  vector<Token> parseTokens(string code);
};
