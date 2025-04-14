#pragma once

#include <any>

#include "Token.h"
#include "Visitor.h"

class Expr {
public:
  virtual std::any accept(Visitor &visitor) = 0;
};

class Unary : public Expr {
public:
  Unary(Token *op, Expr *right) : op(op), right(right) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitUnaryExpr(this);
  }

  Token *op;
  Expr *right;
};

class Logical : public Expr {
public:
  Logical(Expr *left, Token *op, Expr *right)
      : left(left), op(op), right(right) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitLogicalExpr(this);
  }

  Expr *left;
  Token *op;
  Expr *right;
};

class Grouping : public Expr {
public:
  Grouping(Expr *expression) : expression(expression) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitGroupingExpr(this);
  }

  Expr *expression;
};

class Variable : public Expr {
public:
  Variable(Token *name) : name(name) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitVariableExpr(this);
  }

  Token *name;
};

class Literal : public Expr {
public:
  Literal(any value) : value(value) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitLiteralExpr(this);
  }

  any value;
};

class Binary : public Expr {
public:
  Binary(Expr *left, Token *op, Expr *right)
      : left(left), op(op), right(right) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitBinaryExpr(this);
  }

  Expr *left;
  Token *op;
  Expr *right;
};

class Assign : public Expr {
public:
  Assign(Token *name, Expr *value) : name(name), value(value) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitAssignExpr(this);
  }

  Token *name;
  Expr *value;
};
