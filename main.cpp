#include <iostream>

#include "core/Driver.h"
#include "error/ErrorReporter.h"

using namespace std;

int main(int argc, char **args) {
  auto errorReporter = std::make_shared<ErrorReporter>();
  Driver *driver = new Driver(errorReporter);

  if (argc == 1) {
    driver->runPrompt();
  } else if (argc == 2) {
    driver->runFile(args[1]);
  } else if (argc > 2) {
    cerr << "Usage: " << args[0] << " input_file.kol" << endl;
    exit(1);
  }

  return 0;
}
