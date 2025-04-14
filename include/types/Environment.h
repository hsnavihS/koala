#pragma once

#include <any>
#include <string>
#include <unordered_map>

#include "Token.h"

using namespace std;

class Environment {
public:
  Environment() {};
  Environment(Environment *enclosing) : enclosing(enclosing) {};

  /**
   * Because we're not checking for the existence of the variable,
   * we can use the variable declaration to re-define existing variables.
   * TODO: This should be allowed only in the REPL, not in scripts
   */
  void define(const string &name, const any &value) {
    values[name] = value;
  }

  void assign(const string &name, const any &value);

  any get(Token *name);

private:
  Environment *enclosing = nullptr;
  unordered_map<string, any> values = {};
};
