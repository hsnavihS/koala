#pragma once

#include <vector>

#include "Expr.h"
#include "ParseError.h"
#include "Token.h"
#include "TokenType.h"

class Parser {
public:
  Parser(vector<Token> tokens) : tokens(tokens) {}
  Expr *parse();

private:
  vector<Token> tokens;
  int current = 0;

  /**
   * expression -> equality ;
   * equality -> comparison ( ( "!=" | "==" ) comparison )* ;
   * comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
   * term -> factor ( ( "-" | "+" ) factor )* ;
   * factor -> unary ( ( "/" | "*" ) unary )* ;
   * unary -> ( "!" | "-" ) unary | primary ;
   * primary -> NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")"
   * ;
   */
  Expr *expression();
  Expr *equality();
  Expr *comparison();
  Expr *term();
  Expr *factor();
  Expr *unary();
  Expr *primary();

  bool match(std::initializer_list<TokenType> types);
  bool check(TokenType type);
  bool isAtEnd();

  Token *previous();
  ParseError error(Token token, string message);

  void consume(TokenType type, string message);
  void advance();
  void synchronize();
};
