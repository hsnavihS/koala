#pragma once

#include <any>

#include "Expr.h"
#include "Visitor.h"
#include "error/ErrorReporter.h"

using namespace std;

class Interpreter : public Visitor {
public:
  Interpreter(ErrorReporterPtr errorReporter) : errorReporter(errorReporter) {};

  any interpret(Expr *expr);

  any visitLiteralExpr(Literal *expr);
  any visitBinaryExpr(Binary *expr);
  any visitGroupingExpr(Grouping *expr);
  any visitUnaryExpr(Unary *expr);

private:
  ErrorReporterPtr errorReporter;

  bool isTrue(any value);
  bool areEqual(any left, std::any right);
  void validateNumberOperands(Token *token, any left, any right);
};
