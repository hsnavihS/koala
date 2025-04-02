#include <fstream>
#include <iostream>
#include <string>

#include "../../include/Parser.h"

using namespace std;

Parser::Parser() {}

void Parser::runFile(char *filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "Error: Could not open file " << filename << endl;
    exit(1);
  }

  string line;
  string fileContents;

  while (getline(file, line)) {
    fileContents += line + "\n";
  }

  parseTokens(fileContents);

  if (hadError) exit(65);
}

void Parser::runPrompt() {
  while (true) {
    cout << "> ";

    string line;
    getline(cin, line);

    if (line == "exit" || line == "quit" || line.empty()) {
      break;
    }

    parseTokens(line);

    // NOTE: Need to reset the flag after every prompt in REPL mode
    hadError = false;
  }
}

void Parser::reportError(int line, string where, string message) {
  cerr << "[line " << line << "] Error" << where << ": " << message << endl;
  hadError = true;
}

vector<Token> Parser::parseTokens(string code) {
  // TODO: remove this temp structure
  vector<string> futureTokens;
  vector<Token> tokens;

  for (int i = 0; i < code.size(); i++) {
    string buf = "";

    if (isspace(code[i])) {
      continue; // skip all whitespaces
    } else if (isalpha(code[i]) || code[i] == '_') {
      // the first character cannot be a number
      buf.push_back(code[i]);
      i++;

      while (isalnum(code[i]) || code[i] == '_') {
        buf.push_back(code[i]);
        i++;
      }
    } else if (isdigit(code[i])) {
      while (isdigit(code[i])) {
        buf.push_back(code[i]);
        i++;
      }
    }

    futureTokens.push_back(buf);
    buf.clear();
  }

  for (auto token : futureTokens) {
    cout << token << endl;
  }

  return tokens;
}
