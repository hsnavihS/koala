#include <any>
#include <stdexcept>

#include "types/Environment.h"

void Environment::assign(const string &name, const any &value) {
  if (values.find(name) != values.end()) {
    values[name] = value;
    return;
  }

  if (enclosing != nullptr) {
    enclosing->assign(name, value);
    return;
  }

  throw runtime_error("Undefined variable '" + name + "'");
}

any Environment::get(Token *name) {
  if (values.find(name->getLexeme()) != values.end()) {
    return values[name->getLexeme()];
  }

  if (enclosing != nullptr) {
    return enclosing->get(name);
  }

  throw new runtime_error("Undefined variable '" + name->getLexeme() + "'");
}
