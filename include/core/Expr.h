#pragma once

#include "Token.h"
#include "Visitor.h"

using namespace std;

class Expr {
public:
  virtual any accept(Visitor &visitor) = 0;
};

class Unary : public Expr {
public:
  Unary(Token *op, Expr *right) : op(op), right(right) {}

  any accept(Visitor &visitor) override {
    return visitor.visitUnaryExpr(this);
  }

  Token *op;
  Expr *right;
};

class Grouping : public Expr {
public:
  Grouping(Expr *expression) : expression(expression) {}

  any accept(Visitor &visitor) override {
    return visitor.visitGroupingExpr(this);
  }

  Expr *expression;
};

class Literal : public Expr {
public:
  Literal(any value) : value(value) {}

  any accept(Visitor &visitor) override {
    return visitor.visitLiteralExpr(this);
  }

  any value;
};

class Binary : public Expr {
public:
  Binary(Expr *left, Token *op, Expr *right)
      : left(left), op(op), right(right) {}

  any accept(Visitor &visitor) override {
    return visitor.visitBinaryExpr(this);
  }

  Expr *left;
  Token *op;
  Expr *right;
};
