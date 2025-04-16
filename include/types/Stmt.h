#pragma once

#include <any>
#include <vector>

#include "Expr.h"
#include "types/Visitor.h"

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

class Return : public Stmt {
public:
  Return(Token *keyword, Expr *value) : keyword(keyword), value(value) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitReturnStmt(this);
  }

  Token *keyword;
  Expr *value;
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

class Function : public Stmt {
public:
  Function(Token *name, vector<Token *> *parameters, vector<Stmt *> *body)
      : name(name), parameters(parameters), body(body) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitFunctionStmt(this);
  }

  Token *name;
  vector<Token *> *parameters;
  vector<Stmt *> *body;
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
