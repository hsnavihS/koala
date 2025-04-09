#pragma once

#include <exception>
#include <string>

using namespace std;

class RuntimeError : public exception {
public:
  RuntimeError(const string &message, const int lineNumber,
               const int columnNumber)
      : message(message), lineNumber(lineNumber), columnNumber(columnNumber) {}
  const char *what() const noexcept override { return message.c_str(); }
  const int getLineNumber() const { return lineNumber; }
  const int getColumnNumber() const { return columnNumber; }

private:
  string message;
  int lineNumber;
  int columnNumber;
};
