#pragma once

#include <any>

#include "Expr.h"
#include "Stmt.h"
#include "Visitor.h"
#include "error/ErrorReporter.h"

using namespace std;

class Interpreter : public Visitor {
public:
  Interpreter(ErrorReporterPtr errorReporter) : errorReporter(errorReporter) {};
  void interpret(vector<Stmt*> *statements);

private:
  ErrorReporterPtr errorReporter;
  any evaluate(Expr *expr);
  void execute(Stmt *stmt);
  void printValue(any value);

  any visitLiteralExpr(Literal *expr);
  any visitBinaryExpr(Binary *expr);
  any visitGroupingExpr(Grouping *expr);
  any visitUnaryExpr(Unary *expr);

  any visitPrintStmt(Print *stmt);
  any visitExpressionStmt(Expression *stmt);

  bool isTrue(any value);
  bool areEqual(any left, std::any right);
  void validateNumberOperands(Token *token, any left, any right);
};
