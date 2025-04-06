#include <iostream>

#include "core/Driver.h"

using namespace std;

int main(int argc, char **args) {
  Driver driver;

  if (argc == 1) {
    driver.runPrompt();
  } else if (argc == 2) {
    driver.runFile(args[1]);
  } else if (argc > 2) {
    cerr << "Usage: " << args[0] << " input_file.kol" << endl;
    exit(1);
  }

  return 0;
}
