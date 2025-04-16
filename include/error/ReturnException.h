#pragma once

#include <exception>
#include <any>

using namespace std;

class ReturnException : public exception {
public:
  ReturnException(const any &value) : value(value) {}
  any getValue() const { return value; }

private:
  any value;
};
