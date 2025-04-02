#pragma once

#include <string>
#include <vector>

#include "Token.h"

using namespace std;

class Driver {
public:
  Driver();
  void runPrompt();
  void runFile(char *filename);
  void reportError(int line, string where, string message);

private:
  bool hadError = false;
  void run(string code);
};
