#include <iostream>

#include "error/ParseError.h"
#include "core/Parser.h"

using namespace std;

Expr *Parser::parse() {
  try {
    return expression();
  } catch (ParseError &error) {
    return nullptr;
  }
}

Expr *Parser::expression() { return equality(); }

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
    Expr *expr = expression();
    consume(TokenType::RIGHT_PARENTHESIS, "Expect ')' after expression.");
    return new Grouping(expr);
  }

  throw error(tokens.at(current), "Expected expression");
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

void Parser::consume(TokenType type, string message) {
  if (check(type)) {
    advance();
    return;
  }

  cerr << "Error: " << message << endl;
  throw runtime_error("Error: " + message);
}

void Parser::advance() {
  if (!isAtEnd())
    current++;
}

ParseError Parser::error(Token token, string message) {
  cerr << "[line " << token.getLine() << "] Error" << ": " << message << endl;
  return ParseError(message);
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
