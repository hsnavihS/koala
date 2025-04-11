#include "core/Parser.h"
#include "core/Stmt.h"
#include "error/ParserError.h"

using namespace std;

vector<Stmt *> *Parser::parse() {
  vector<Stmt *> *statements = new vector<Stmt *>;

  while (!isAtEnd()) {
    statements->push_back(declaration());
  }

  return statements;
}

Stmt *Parser::declaration() {
  try {
    if (match({TokenType::VAR})) {
      return varDeclaration();
    }
    return statement();
  } catch (ParserError &error) {
    synchronize();
  }
  return nullptr;
}

Stmt *Parser::varDeclaration() {
  Token *name =
      consume(TokenType::IDENTIFIER, previous(), "Expected variable name");

  Expr *initializer = nullptr;
  if (match({TokenType::EQUAL})) {
    initializer = expression();
  }

  consume(TokenType::SEMICOLON, previous(),
          "Expected ';' after variable declaration");
  return new Var(name, initializer);
}

Stmt *Parser::statement() {
  if (match({TokenType::PRINT})) {
    return printStatement();
  }

  return expressionStatement();
}

Stmt *Parser::printStatement() {
  Expr *value = expression();
  consume(TokenType::SEMICOLON, previous(), "Expected ';'");
  return new Print(value);
}

Stmt *Parser::expressionStatement() {
  Expr *value = expression();
  consume(TokenType::SEMICOLON, previous(), "Expected ';'");
  return new Expression(value);
}

Expr *Parser::expression() { return assignment(); }

Expr *Parser::assignment() {
  Expr *expr = equality();

  if (match({TokenType::EQUAL})) {
    Token *equals = previous();
    Expr *value = assignment();

    // dynamic casting in C++ is the equivalent of using 'instanceof' in Java
    if (Expr *var = dynamic_cast<Variable *>(expr)) {
      Variable *variable = static_cast<Variable *>(var);
      Token *name = variable->name;
      return new Assign(name, value);
    }

    error(equals, "Invalid assignment target");
  }

  /**
   * Because assignments can be printed as well:
   *   var a = 1;
   *   print a = 2; // this should work
   */
  return expr;
}

Expr *Parser::equality() {
  // LHS
  Expr *expr = comparison();

  while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
    // operator
    Token *op = previous();
    // RHS
    Expr *right = comparison();
    expr = new Binary(expr, op, right);
  }

  return expr;
}

Expr *Parser::comparison() {
  Expr *expr = term();

  while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                TokenType::LESS_EQUAL})) {
    Token *op = previous();
    Expr *right = term();
    expr = new Binary(expr, op, right);
  }

  return expr;
}

Expr *Parser::term() {
  Expr *expr = factor();

  while (match({TokenType::MINUS, TokenType::PLUS})) {
    Token *op = previous();
    Expr *right = factor();
    expr = new Binary(expr, op, right);
  }

  return expr;
}

Expr *Parser::factor() {
  Expr *expr = unary();

  while (match({TokenType::SLASH, TokenType::STAR})) {
    Token *op = previous();
    Expr *right = unary();
    expr = new Binary(expr, op, right);
  }

  return expr;
}

Expr *Parser::unary() {
  if (match({TokenType::BANG, TokenType::MINUS})) {
    Token *op = previous();
    Expr *right = unary();
    return new Unary(op, right);
  }

  return primary();
}

Expr *Parser::primary() {
  // nil
  if (match({TokenType::NIL})) {
    return new Literal(nullptr);
  }

  // boolean
  if (match({TokenType::FALSE})) {
    return new Literal(false);
  }
  if (match({TokenType::TRUE})) {
    return new Literal(true);
  }

  // number or string
  if (match({TokenType::NUMBER, TokenType::STRING})) {
    if (previous()->getType() == TokenType::NUMBER) {
      return new Literal(any_cast<int>(previous()->getLiteral()));
    } else {
      return new Literal(any_cast<string>(previous()->getLiteral()));
    }
  }

  // group
  if (match({TokenType::LEFT_PARENTHESIS})) {
    Token *startingToken = previous();
    Expr *expr = expression();
    consume(TokenType::RIGHT_PARENTHESIS, startingToken,
            "No matching parenthesis found");
    return new Grouping(expr);
  }

  // variable
  if (match({TokenType::IDENTIFIER})) {
    return new Variable(previous());
  }

  throw error(&tokens.at(current), "Expected an expression");
}

bool Parser::match(initializer_list<TokenType> types) {
  for (TokenType type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }
  return false;
}

bool Parser::check(TokenType type) {
  if (isAtEnd())
    return false;
  return tokens.at(current).getType() == type;
}

bool Parser::isAtEnd() {
  return tokens.at(current).getType() == TokenType::END_OF_FILE;
}

Token *Parser::previous() { return &tokens.at(current - 1); }

Token *Parser::consume(TokenType type, Token *startingToken, string message) {
  if (check(type)) {
    return advance();
  }
  throw error(startingToken, message);
}

Token *Parser::advance() {
  if (!isAtEnd())
    current++;
  return previous();
}

ParserError Parser::error(Token *token, string message) {
  errorReporter->report(token->getLine(), token->getColumn(), message);
  return ParserError(message);
}

void Parser::synchronize() {
  advance();

  while (!isAtEnd()) {
    if (previous()->getType() == TokenType::SEMICOLON) {
      return;
    }

    switch (previous()->getType()) {
    case TokenType::CLASS:
    case TokenType::FUNC:
    case TokenType::VAR:
    case TokenType::FOR:
    case TokenType::IF:
    case TokenType::WHILE:
    case TokenType::PRINT:
    case TokenType::RETURN:
      return;
    default:
      break;
    }

    advance();
  }
}
