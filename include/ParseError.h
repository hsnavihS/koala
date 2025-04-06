#pragma once

#include <exception>
#include <string>

using namespace std;

class ParseError : public exception {
public:
  ParseError(const string &message) : message(message) {}
  const char *what() const noexcept override { return message.c_str(); }

private:
  string message;
};
