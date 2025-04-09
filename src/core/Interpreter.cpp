#include <any>

#include "core/Interpreter.h"
#include "error/RuntimeError.h"

using namespace std;

any Interpreter::interpret(Expr *expr) {
  try {
    return expr->accept(*this);
  } catch (const RuntimeError &e) {
    errorReporter->reportRuntimeError(e.getLineNumber(), e.getColumnNumber(),
                                      e.what());
  }
  return nullptr;
}

any Interpreter::visitLiteralExpr(Literal *expr) { return expr->value; }

any Interpreter::visitGroupingExpr(Grouping *expr) {
  return interpret(expr->expression);
}

any Interpreter::visitUnaryExpr(Unary *expr) {
  any right = interpret(expr->right);

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
  any left = interpret(expr->left);
  any right = interpret(expr->right);

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

// Only false, nil and 0 are false
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
