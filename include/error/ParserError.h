#pragma once

#include <exception>
#include <string>

using namespace std;

class ParserError : public exception {
public:
  ParserError(const string &message) : message(message) {}
  const char *what() const noexcept override { return message.c_str(); }

private:
  string message;
};
