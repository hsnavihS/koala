#include <iostream>

#include "include/Parser.h"

using namespace std;

int main(int argc, char **args) {
  Parser parser;

  if (argc == 1) {
    parser.runPrompt();
  } else if (argc == 2) {
    parser.runFile(args[1]);
  } else if (argc > 2) {
    cerr << "Usage: " << args[0] << " input_file.kol" << endl;
    exit(1);
  }

  return 0;
}
