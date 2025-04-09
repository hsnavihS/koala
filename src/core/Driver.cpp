#include <fstream>
#include <iostream>
#include <string>

#include "core/Driver.h"
#include "core/Expr.h"
#include "core/Interpreter.h"
#include "core/Lexer.h"
#include "core/Parser.h"
#include "utils/AstPrinter.h"

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
  Expr *expr = parser->parse();
  AstPrinter *printer = new AstPrinter();

  if (errorReporter->errorDetected()) {
    return;
  }

  static Interpreter *interpreter = new Interpreter(errorReporter);
  any value = interpreter->interpret(expr);
  printValue(value);
}

void Driver::printValue(any value) {
  // NOTE: Currently only int works, maybe extend this to support double, long
  // and long long as well?
  if (value.type() == typeid(int)) {
    cout << any_cast<int>(value) << endl;
  } else if (value.type() == typeid(string)) {
    cout << any_cast<string>(value) << endl;
  } else if (value.type() == typeid(bool)) {
    cout << (any_cast<bool>(value) ? "true" : "false") << endl;
  } else if (value.type() == typeid(nullptr_t)) {
    cout << "nil" << endl;
  }
}
