#pragma once

#include <any>

#include "Expr.h"
#include "Visitor.h"

class Stmt {
public:
  virtual std::any accept(Visitor &visitor) = 0;
};

class Print : public Stmt {
public:
  Print(Expr *expression) : expression(expression) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitPrintStmt(this);
  }

  Expr *expression;
};

class Expression : public Stmt {
public:
  Expression(Expr *expression) : expression(expression) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitExpressionStmt(this);
  }

  Expr *expression;
};
