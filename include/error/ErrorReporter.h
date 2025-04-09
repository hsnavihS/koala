#pragma once

#include <memory>
#include <string>
#include <vector>

using namespace std;

class ErrorReporter {
public:
  ErrorReporter() {}

  void report(const int line, const int column, const string message);
  void reportRuntimeError(const int line, const int column,
                          const string message);
  void setupErrorReporter(const string code, const string filename = "");
  bool errorDetected() { return hadError; }
  bool runtimeErrorDetected() { return hadRuntimeError; }
  void reset() { hadError = false; }

private:
  string code;
  string filename = "";
  vector<string> lines;
  bool hadError = false;
  bool hadRuntimeError = false;

  void printError(const int line, const int column, const string message,
                  bool isRuntimeError = false);
  void split(string s, vector<string> *result, string delimiter);
};

using ErrorReporterPtr = shared_ptr<ErrorReporter>;
