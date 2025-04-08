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
    /**
     * Red: \033[31m
     * Bold: \033[1m
     * Red + bold: \033[1;31m
     * Reset: \033[0m
     */
    cerr << "     Error: " << "\033[1;31m" << message << "\033[0m\n";

    if (filename != "") {
      cerr << "       --> " << filename << ":" << line << ":" << column << "\n";
    }

    cerr << "       |" << "\n"
         << "     " << line << " | " << "\033[31m" << lines[line - 1]
         << "\033[0m\n"
         << "       | " << string(column - 1, ' ') << "^" << "\n";

    hadError = true;
  }

  void setupErrorReporter(const string code, const string filename = "") {
    this->code = code;
    lines.clear();
    split(code, &lines, "\n");
    this->filename = filename;
  }

  bool errorDetected() { return hadError; }
  void reset() { hadError = false; }

private:
  string code;
  string filename = "";
  vector<string> lines;
  bool hadError = false;

  // TODO: Move this method to a utility class and use it in
  // createAstClasses.cpp as well
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
