#pragma once

#include <any>

#include "Visitor.h"
#include <vector>

#include "Expr.h"

class Stmt {
public:
  virtual std::any accept(Visitor &visitor) = 0;
};

class While : public Stmt {
public:
  While(Expr *condition, Stmt *body) : condition(condition), body(body) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitWhileStmt(this);
  }

  Expr *condition;
  Stmt *body;
};

class Print : public Stmt {
public:
  Print(Expr *expression) : expression(expression) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitPrintStmt(this);
  }

  Expr *expression;
};

class If : public Stmt {
public:
  If(Expr *condition, Stmt *thenBranch, Stmt *elseBranch)
      : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitIfStmt(this);
  }

  Expr *condition;
  Stmt *thenBranch;
  Stmt *elseBranch;
};

class Var : public Stmt {
public:
  Var(Token *name, Expr *initializer) : name(name), initializer(initializer) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitVarStmt(this);
  }

  Token *name;
  Expr *initializer;
};

class Expression : public Stmt {
public:
  Expression(Expr *expression) : expression(expression) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitExpressionStmt(this);
  }

  Expr *expression;
};

class Block : public Stmt {
public:
  Block(vector<Stmt *> *statements) : statements(statements) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitBlockStmt(this);
  }

  vector<Stmt *> *statements;
};
