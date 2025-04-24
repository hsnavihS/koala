#include <fstream>
#include <iostream>
#include <string>

#include "core/Driver.h"
#include "core/Interpreter.h"
#include "core/Lexer.h"
#include "core/Parser.h"

using namespace std;

void Driver::runFile(char *filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    errorReporter->report(0, 0, "Could not open file: " + string(filename));
    exit(1);
  }

  string line;
  string fileContents;

  while (getline(file, line)) {
    fileContents += line + "\n";
  }

  run(fileContents, filename);

  if (errorReporter->errorDetected())
    exit(65);
  if (errorReporter->runtimeErrorDetected())
    exit(70);
}

void Driver::runPrompt() {
  while (true) {
    cout << "> ";

    string line;
    getline(cin, line);

    if (line == "exit" || line == "quit" || line.empty()) {
      cout << "Received '" << line << "'" << endl;
      break;
    }

    run(line);

    // NOTE: Need to reset the flag after every prompt in REPL mode
    errorReporter->reset();
  }
}

void Driver::run(string code, string filename) {
  errorReporter->setupErrorReporter(code, filename);

  Lexer *lexer = new Lexer(code, errorReporter);
  vector<Token> tokens = lexer->generateTokens();

  Parser *parser = new Parser(tokens, errorReporter);
  vector<Stmt *> *statements = parser->parse();

  if (errorReporter->errorDetected()) {
    return;
  }

  static Interpreter *interpreter = new Interpreter(errorReporter);
  interpreter->interpret(statements);
}
