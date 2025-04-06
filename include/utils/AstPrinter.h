#pragma once

#include <any>
#include <string>

#include "core/Expr.h"
#include "core/Visitor.h"

using namespace std;

class AstPrinter : public Visitor {
public:
  string print(Expr &expr) { return any_cast<string>(expr.accept(*this)); };

  any visitUnaryExpr(Unary *expr) override {
    return any_cast<string>(parenthesize(expr->op->getLexeme(), expr->right));
  }

  any visitGroupingExpr(Grouping *expr) override {
    return any_cast<string>(parenthesize("group", expr->expression));
  }

  any visitBinaryExpr(Binary *expr) override {
    return any_cast<string>(
        parenthesize(expr->op->getLexeme(), expr->left, expr->right));
  }

  any visitLiteralExpr(Literal *expr) override {
    if (!expr->value.has_value())
      return string("nil");

    // TODO: It would probably be better to map each of these individual
    // datatypes to an integer and use a switch statement here
    if (expr->value.type() == typeid(int)) {
      return to_string(any_cast<int>(expr->value));
    } else if (expr->value.type() == typeid(double)) {
      return to_string(any_cast<double>(expr->value));
    } else if (expr->value.type() == typeid(bool)) {
      return any_cast<bool>(expr->value) ? "true" : "false";
    } else if (expr->value.type() == typeid(string)) {
      return any_cast<string>(expr->value);
    } else if (expr->value.type() == typeid(nullptr)) {
      return string("nil");
    }

    return string("unknown");
  }

private:
  template <typename... Args> string parenthesize(string name, Args... args) {
    string result = "(" + name;

    for (auto arg : {args...}) {
      result += " ";
      result += any_cast<string>(arg->accept(*this));
    }

    result += ")";
    return result;
  }
};
