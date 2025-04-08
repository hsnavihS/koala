#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class ErrorReporter {
public:
  ErrorReporter() {}
  void report(const int line, const int column, const string message) {
    cerr << "[line " << line << "]: " << message << endl;

    cerr << lines[line - 1] << endl;
    cerr << string(column - 1, ' ') << "^" << endl;

    hadError = true;
  }

  void setCode(const string code) {
    this->code = code;
    lines.clear();
    split(code, &lines, "\n");
  }

  bool errorDetected() { return hadError; }
  void reset() { hadError = false; }

private:
  string code;
  vector<string> lines;
  bool hadError = false;

  // TODO: Move this method to a utility class and use it in createAstClasses.cpp as well
  void split(string s, vector<string> *result, string delimiter) {
    int delimiter_length = delimiter.length();

    while (s.size()) {
      int position = s.find(delimiter);
      result->push_back(s.substr(0, position));
      s = s.substr(position + delimiter_length);
      if (position == string::npos) {
        break;
      }
    }
  }
};

using ErrorReporterPtr = shared_ptr<ErrorReporter>;
