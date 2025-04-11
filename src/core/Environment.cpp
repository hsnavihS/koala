#include <stdexcept>

#include "core/Environment.h"

void Environment::assign(const string &name, const any &value) {
  if (values.find(name) != values.end()) {
    values[name] = value;
    return;
  }

  throw runtime_error("Undefined variable '" + name + "'");
}

any Environment::get(Token *name) {
  if (values.find(name->getLexeme()) != values.end()) {
    return values[name->getLexeme()];
  }

  throw new runtime_error("Undefined variable '" + name->getLexeme() + "'");
}
