#include <fstream>
#include <iostream>
#include <string>

#include "../../include/AstPrinter.h"
#include "../../include/Driver.h"
#include "../../include/Expr.h"
#include "../../include/Lexer.h"
#include "../../include/Parser.h"

using namespace std;

Driver::Driver() {}

void Driver::runFile(char *filename) {
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

  run(fileContents);

  if (hadError)
    exit(65);
}

void Driver::runPrompt() {
  while (true) {
    cout << "> ";

    string line;
    getline(cin, line);

    if (line == "exit" || line == "quit" || line.empty()) {
      break;
    }

    run(line);

    // NOTE: Need to reset the flag after every prompt in REPL mode
    hadError = false;
  }
}

void Driver::reportError(int line, string where, string message) {
  cerr << "[line " << line << "] Error" << where << ": " << message << endl;
  hadError = true;
}

void Driver::run(string code) {
  Lexer *lexer = new Lexer(code);
  vector<Token> tokens = lexer->generateTokens();

  Parser *parser = new Parser(tokens);
  Expr *expr = parser->parse();
  AstPrinter *printer = new AstPrinter();

  cout << printer->print(*expr) << endl;
}
