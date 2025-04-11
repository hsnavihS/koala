#pragma once

#include <any>

using namespace std;

// forward declarations of used classes
class Unary;
class Grouping;
class Literal;
class Binary;
class Print;
class Expression;
class Var;
class Variable;

class Visitor {
public:
  virtual any visitUnaryExpr(Unary *expr) = 0;
  virtual any visitGroupingExpr(Grouping *expr) = 0;
  virtual any visitLiteralExpr(Literal *expr) = 0;
  virtual any visitBinaryExpr(Binary *expr) = 0;
  virtual any visitVariableExpr(Variable *expr) = 0;

  // Could've implemented another interface for statements, but I think this is
  // fine for now
  virtual any visitPrintStmt(Print *stmt) = 0;
  virtual any visitExpressionStmt(Expression *stmt) = 0;
  virtual any visitVarStmt(Var *stmt) = 0;
};
