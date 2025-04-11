#pragma once

#include <any>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "Token.h"

using namespace std;

class Environment {
public:
  /**
   * Because we're not checking for the existence of the variable,
   * we can use the variable declaration to re-define existing variables.
   * TODO: This should be allowed only in the REPL, not in scripts
   */
  void define(const string &name, const any &value) { values[name] = value; }

  any get(Token *name) {
    if (values.find(name->getLexeme()) != values.end()) {
      return values[name->getLexeme()];
    }

    throw new runtime_error("Undefined variable '" + name->getLexeme() + "'");
  }

private:
  unordered_map<string, any> values = {};
};
