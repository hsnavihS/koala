#pragma once

#include <any>

using namespace std;

// forward declarations of used Expr classes
class Unary;
class Grouping;
class Literal;
class Binary;
class Variable;
class Assign;
class Logical;

// forward declarations of used Stmt classes
class If;
class Var;
class Print;
class While;
class Block;
class Expression;

class Visitor {
public:
  virtual any visitUnaryExpr(Unary *expr) = 0;
  virtual any visitGroupingExpr(Grouping *expr) = 0;
  virtual any visitLiteralExpr(Literal *expr) = 0;
  virtual any visitBinaryExpr(Binary *expr) = 0;
  virtual any visitVariableExpr(Variable *expr) = 0;
  virtual any visitAssignExpr(Assign *expr) = 0;
  virtual any visitLogicalExpr(Logical *expr) = 0;

  // Could've implemented another interface for statements, but I think this is
  // fine for now
  virtual any visitBlockStmt(Block *stmt) = 0;
  virtual any visitPrintStmt(Print *stmt) = 0;
  virtual any visitExpressionStmt(Expression *stmt) = 0;
  virtual any visitVarStmt(Var *stmt) = 0;
  virtual any visitIfStmt(If *stmt) = 0;
  virtual any visitWhileStmt(While *stmt) = 0;
};
