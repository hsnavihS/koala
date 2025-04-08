#include <iostream>

#include "core/Token.h"
#include "core/TokenType.h"
#include "utils/AstPrinter.h"

using namespace std;

int main() {
  Expr *expression = new Binary(
      new Unary(new Token(TokenType::MINUS, "-", nullptr, 1, 1), new Literal(123)),
      new Token(TokenType::STAR, "*", nullptr, 1, 2),
      new Grouping(new Literal(nullptr)));

  AstPrinter printer;
  cout << printer.print(*expression) << endl;
};
