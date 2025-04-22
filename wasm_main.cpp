#include <cstring>
#include <iostream>
#include <string>

#include "core/Driver.h"
#include "error/ErrorReporter.h"
#include "error/ReturnException.h"

extern "C" {
void interpret(const char *input) {
  static std::string result;
  auto errorReporter = std::make_shared<ErrorReporter>();
  Driver *driver = new Driver(errorReporter);
  try {
    driver->run(input);
  } catch (const ReturnException &ret) {
    // This is a normal return from a function, not an error
    // We can ignore it as the return value is already handled by the
    // interpreter
    return;
  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << std::endl;
    return;
  } catch (const std::any &error) {
    std::cerr << "Error: " << error.type().name() << std::endl;
    return;
  } catch (...) {
    std::cerr << "Unknown error occurred." << std::endl;
    return;
  }
}
}
