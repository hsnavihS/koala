#pragma once

#include <iostream>
#include <memory>
#include <string>

using namespace std;

class ErrorReporter {
public:
  ErrorReporter() {}
  void report(const int line, const string message) {
    cerr << "[line " << line << "]: " << message << endl;
    hadError = true;
  }
  bool errorDetected() { return hadError; }
  void reset() { hadError = false; }

private:
  bool hadError = false;
};

using ErrorReporterPtr = shared_ptr<ErrorReporter>;
