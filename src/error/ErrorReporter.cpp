#include <iostream>

#include <error/ErrorReporter.h>

void ErrorReporter::report(const int line, const int column,
                           const string message) {
  printError(line, column, message);
  hadError = true;
}

void ErrorReporter::reportRuntimeError(const int line, const int column,
                                       const string message) {
  printError(line, column, message, true);
  hadRuntimeError = true;
}

void ErrorReporter::setupErrorReporter(const string code,
                                       const string filename) {
  this->code = code;
  lines.clear();
  split(code, &lines, "\n");
  this->filename = filename;
}

void ErrorReporter::printError(const int line, const int column,
                               const string message, bool isRuntimeError) {
  if (isRuntimeError) {
    cerr << "     Runtime Error: " << "\033[1;31m" << message << "\033[0m\n";
  } else {
    cerr << "     Error: " << "\033[1;31m" << message << "\033[0m\n";
  }

  if (filename != "") {
    cerr << "       --> " << filename << ":" << line << ":" << column << "\n";
  }

  cerr << "       |" << "\n"
       << "     " << line << " | " << "\033[31m" << lines[line - 1]
       << "\033[0m\n"
       << "       | " << string(column - 1, ' ') << "^" << "\n";
}

// TODO: Move this method to a utility class and use it in
// createAstClasses.cpp as well
void ErrorReporter::split(string s, vector<string> *result, string delimiter) {
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
