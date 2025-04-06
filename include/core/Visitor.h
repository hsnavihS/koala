#pragma once

#include <any>

using namespace std;

// forward declarations of used classes
class Unary;
class Grouping;
class Literal;
class Binary;


class Visitor {
public:
  virtual any visitUnaryExpr(Unary *expr) = 0;
  virtual any visitGroupingExpr(Grouping *expr) = 0;
  virtual any visitLiteralExpr(Literal *expr) = 0;
  virtual any visitBinaryExpr(Binary *expr) = 0;
};
