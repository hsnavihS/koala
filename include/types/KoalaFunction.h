#pragma once

#include "Callable.h"
#include "Environment.h"
#include "Stmt.h"
#include "core/Interpreter.h"
#include "error/ReturnException.h"

using namespace std;

class KoalaFunction : public Callable {
public:
  KoalaFunction(Function *declaration) : declaration(declaration) {}

  int arity() { return declaration->parameters->size(); }

  any call(Interpreter *interpreter, vector<any> arguments) {
    Environment *previous = interpreter->environment;
    // Create a new environment for this function call and set it as the current
    Environment *environment = new Environment(previous);
    interpreter->environment = environment;

    for (int i = 0; i < declaration->parameters->size(); i++) {
      environment->define(declaration->parameters->at(i)->getLexeme(),
                          arguments.at(i));
    }

    try {
      interpreter->executeBlock(declaration->body, environment);
    } catch (ReturnException value) {
      // Restore the previous environment
      interpreter->environment = previous;
      return value.getValue();
    }

    // Restore the previous environment
    interpreter->environment = previous;
    return nullptr;
  }

  string toString() {
    return "<runtime function: " + declaration->name->getLexeme() + ">";
  }

private:
  Function *declaration;
};
