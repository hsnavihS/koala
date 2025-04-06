#include "../include/AstPrinter.h"
#include "../include/Token.h"
#include "../include/TokenType.h"

using namespace std;

int main() {
  Expr *expression = new Binary(
    new Unary(
      new Token(TokenType::MINUS, "-", nullptr, 1),
      new Literal(123)
    ),
    new Token(TokenType::STAR, "*", nullptr, 1),
    new Grouping(
      new Literal(nullptr)
    )
  );

  AstPrinter printer;
  cout << printer.print(*expression) << endl;
};
