#include <any>

#include <iostream>

#include "core/Interpreter.h"
#include "error/ReturnException.h"
#include "error/RuntimeError.h"
#include "types/Callable.h"
#include "types/KoalaFunction.h"
#include "types/Stmt.h"

using namespace std;

void Interpreter::interpret(vector<Stmt *> *statements) {
  try {
    for (auto &statement : *statements) {
      execute(statement);
    }
  } catch (const RuntimeError &error) {
    errorReporter->reportRuntimeError(error.getLineNumber(),
                                      error.getColumnNumber(), error.what());
  }
}

void Interpreter::execute(Stmt *stmt) { stmt->accept(*this); }

any Interpreter::evaluate(Expr *expr) { return expr->accept(*this); }

any Interpreter::visitLiteralExpr(Literal *expr) { return expr->value; }

any Interpreter::visitGroupingExpr(Grouping *expr) {
  return evaluate(expr->expression);
}

any Interpreter::visitUnaryExpr(Unary *expr) {
  any right = evaluate(expr->right);

  switch (expr->op->getType()) {
  case TokenType::MINUS:
    if (right.type() == typeid(int))
      return -any_cast<int>(right);
    else
      errorReporter->report(expr->op->getLine(), expr->op->getColumn(),
                            "Operand must be a number.");
  case TokenType::BANG:
    return !isTrue(right);
  default:
    break;
  }

  return nullptr;
}

any Interpreter::visitBinaryExpr(Binary *expr) {
  any left = evaluate(expr->left);
  any right = evaluate(expr->right);

  switch (expr->op->getType()) {
  case TokenType::MINUS:
    validateNumberOperands(expr->op, left, right);
    return any_cast<int>(left) - std::any_cast<int>(right);
  case TokenType::SLASH:
    validateNumberOperands(expr->op, left, right);
    return any_cast<int>(left) / std::any_cast<int>(right);
  case TokenType::STAR:
    validateNumberOperands(expr->op, left, right);
    return any_cast<int>(left) * std::any_cast<int>(right);
  case TokenType::GREATER:
    validateNumberOperands(expr->op, left, right);
    return any_cast<int>(left) > std::any_cast<int>(right);
  case TokenType::GREATER_EQUAL:
    validateNumberOperands(expr->op, left, right);
    return any_cast<int>(left) >= std::any_cast<int>(right);
  case TokenType::LESS:
    validateNumberOperands(expr->op, left, right);
    return any_cast<int>(left) < std::any_cast<int>(right);
  case TokenType::LESS_EQUAL:
    validateNumberOperands(expr->op, left, right);
    return any_cast<int>(left) <= std::any_cast<int>(right);

  case TokenType::PLUS:
    if (left.type() == typeid(int) && right.type() == typeid(int))
      return any_cast<int>(left) + std::any_cast<int>(right);
    else if (left.type() == typeid(string) && right.type() == typeid(string))
      return any_cast<string>(left) + std::any_cast<string>(right);

    throw RuntimeError("Operands must be two numbers or two strings",
                       expr->op->getLine(), expr->op->getColumn());
  case TokenType::EQUAL_EQUAL:
    return areEqual(left, right);
  case TokenType::BANG_EQUAL:
    return !areEqual(left, right);
  default:
    break;
  }

  return nullptr;
}

any Interpreter::visitVariableExpr(Variable *expr) {
  return environment->get(expr->name);
}

any Interpreter::visitAssignExpr(Assign *expr) {
  any value = evaluate(expr->value);
  environment->assign(expr->name->getLexeme(), value);
  return value;
}

any Interpreter::visitLogicalExpr(Logical *expr) {
  any left = evaluate(expr->left);

  if (expr->op->getType() == TokenType::OR) {
    if (isTrue(left)) {
      return left;
    }
  } else {
    if (!isTrue(left)) {
      return left;
    }
  }

  return evaluate(expr->right);
}

any Interpreter::visitCallExpr(Call *expr) {
  any callee = evaluate(expr->callee);

  cout << callee.type().name() << endl;

  vector<any> arguments;
  for (auto &argument : *expr->arguments) {
    arguments.push_back(evaluate(argument));
  }

  try {
    Callable *function = any_cast<Callable *>(callee);

    if (arguments.size() != function->arity()) {
      throw RuntimeError("Expected " + to_string(function->arity()) +
                             " arguments but got " +
                             to_string(arguments.size()),
                         expr->paren->getLine(), expr->paren->getColumn());
    }

    any returnValue = function->call(this, arguments);
    return returnValue;
  } catch (const bad_any_cast &) {
    throw RuntimeError("Can only call functions and classes.",
                       expr->paren->getLine(), expr->paren->getColumn());
  }
}

any Interpreter::visitPrintStmt(Print *print) {
  any value = evaluate(print->expression);
  printValue(value);
  return value;
}

any Interpreter::visitExpressionStmt(Expression *stmt) {
  evaluate(stmt->expression);
  return nullptr;
}

any Interpreter::visitBlockStmt(Block *stmt) {
  executeBlock(stmt->statements, new Environment(environment));
  return nullptr;
}

any Interpreter::visitIfStmt(If *stmt) {
  if (isTrue(evaluate(stmt->condition))) {
    execute(stmt->thenBranch);
  } else if (stmt->elseBranch != nullptr) {
    execute(stmt->elseBranch);
  }
  return nullptr;
}

any Interpreter::visitWhileStmt(While *stmt) {
  while (isTrue(evaluate(stmt->condition))) {
    execute(stmt->body);
  }
  return nullptr;
}

/**
 * If a variable hasn't explicitly been initialized, we'll set it to nil
 */
any Interpreter::visitVarStmt(Var *stmt) {
  any value = nullptr;
  if (stmt->initializer != nullptr) {
    value = evaluate(stmt->initializer);
  }

  environment->define(stmt->name->getLexeme(), value);
  return nullptr;
}

any Interpreter::visitFunctionStmt(Function *stmt) {
  // NOTE: Since C++ does not have the concept of an interface, we need to cast
  // the function as a callable before storing it
  Callable *function = new KoalaFunction(stmt);
  this->environment->define(stmt->name->getLexeme(), function);
  cout << stmt->name->getLexeme() << endl;
  return nullptr;
}

any Interpreter::visitReturnStmt(Return *stmt) {
  any value = nullptr;
  if (stmt->value != nullptr) {
    value = evaluate(stmt->value);
  }

  throw ReturnException(value);
}

/** Helpers **/

/**
 * Only false, nil and 0 are false
 */
bool Interpreter::isTrue(any value) {
  if (value.type() == typeid(nullptr_t))
    return false;
  else if (value.type() == typeid(bool))
    return any_cast<bool>(value);
  else if (value.type() == typeid(int))
    return any_cast<int>(value) != 0;

  return true;
}

bool Interpreter::areEqual(std::any left, std::any right) {
  if (left.type() != right.type())
    return false;

  if (left.type() == typeid(int) && right.type() == typeid(int))
    return any_cast<int>(left) == any_cast<int>(right);
  else if (left.type() == typeid(string) && right.type() == typeid(string))
    return any_cast<string>(left) == any_cast<string>(right);
  else if (left.type() == typeid(bool) && right.type() == typeid(bool))
    return any_cast<bool>(left) == any_cast<bool>(right);
  else if (left.type() == typeid(nullptr_t) &&
           right.type() == typeid(nullptr_t))
    return true;

  return false;
}

void Interpreter::validateNumberOperands(Token *token, any left, any right) {
  if (left.type() == typeid(int) && right.type() == typeid(int))
    return;

  throw RuntimeError("Operands must be numbers", token->getLine(),
                     token->getColumn());
}

void Interpreter::printValue(any value) {
  // NOTE: Currently only int works, maybe extend this to support double, long
  // and long long as well?
  if (value.type() == typeid(int)) {
    cout << any_cast<int>(value) << endl;
  } else if (value.type() == typeid(string)) {
    cout << any_cast<string>(value) << endl;
  } else if (value.type() == typeid(bool)) {
    cout << (any_cast<bool>(value) ? "true" : "false") << endl;
  } else if (value.type() == typeid(nullptr_t)) {
    cout << "nil" << endl;
  }
}

void Interpreter::executeBlock(vector<Stmt *> *statements,
                               Environment *environment) {
  Environment *parent = this->environment;

  try {
    this->environment = environment;

    for (auto statement : *statements) {
      execute(statement);
    }
  } catch (const RuntimeError &error) {
    throw error;
  }

  this->environment = parent;
}
