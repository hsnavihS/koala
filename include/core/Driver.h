#pragma once

#include <any>
#include <string>

#include "error/ErrorReporter.h"

using namespace std;

class Driver {
public:
  Driver(ErrorReporterPtr errorReporter) : errorReporter(errorReporter) {}
  void runPrompt();
  void runFile(char *filename);
  void reportError(int line, string where, string message);
  void run(string code, string filename = "");

private:
  ErrorReporterPtr errorReporter;
  void printValue(any value);
};
