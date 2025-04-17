#pragma once

#include <any>

#include "error/ErrorReporter.h"
#include "types/Environment.h"
#include "types/Expr.h"
#include "types/Stmt.h"
#include "types/Visitor.h"

using namespace std;

class Interpreter : public Visitor {
public:
  Interpreter(ErrorReporterPtr errorReporter) {
    this->errorReporter = errorReporter;
  };
  void interpret(vector<Stmt *> *statements);

  ErrorReporterPtr errorReporter;
  Environment *globals = new Environment();
  Environment *environment = globals;
  Environment *getGlobals() { return globals; }

  any evaluate(Expr *expr);
  void execute(Stmt *stmt);
  void printValue(any value);

  any visitLiteralExpr(Literal *expr);
  any visitBinaryExpr(Binary *expr);
  any visitGroupingExpr(Grouping *expr);
  any visitUnaryExpr(Unary *expr);
  any visitVariableExpr(Variable *expr);
  any visitAssignExpr(Assign *expr);
  any visitLogicalExpr(Logical *expr);
  any visitCallExpr(Call *expr);

  any visitVarStmt(Var *stmt);
  any visitBlockStmt(Block *stmt);
  any visitPrintStmt(Print *stmt);
  any visitExpressionStmt(Expression *stmt);
  any visitIfStmt(If *stmt);
  any visitWhileStmt(While *stmt);
  any visitFunctionStmt(Function *stmt);
  any visitReturnStmt(Return *stmt);

  bool isTrue(any value);
  bool areEqual(any left, std::any right);
  void validateNumberOperands(Token *token, any left, any right);
  void executeBlock(vector<Stmt *> *statements, Environment *environment);
};
