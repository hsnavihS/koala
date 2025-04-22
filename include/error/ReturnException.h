#pragma once

#include <any>
#include <exception>

using namespace std;

class ReturnException : public exception {
public:
  ReturnException(const any &value) : value(value) {}
  any getValue() const { return value; }

private:
  any value;
};
