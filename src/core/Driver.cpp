#include <fstream>
#include <iostream>
#include <string>

#include "core/Driver.h"
#include "core/Expr.h"
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

  run(fileContents);

  if (errorReporter->errorDetected())
    exit(65);
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

void Driver::run(string code) {
  errorReporter->setCode(code);

  Lexer *lexer = new Lexer(code, errorReporter);
  vector<Token> tokens = lexer->generateTokens();

  Parser *parser = new Parser(tokens, errorReporter);
  Expr *expr = parser->parse();
  AstPrinter *printer = new AstPrinter();

  if (errorReporter->errorDetected()) {
    return;
  }

  cout << printer->print(*expr) << endl;
}
