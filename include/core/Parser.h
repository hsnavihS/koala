#pragma once

#include <vector>

#include "Expr.h"
#include "Token.h"
#include "TokenType.h"
#include "error/ErrorReporter.h"
#include "error/ParserError.h"

class Parser {
public:
  Parser(vector<Token> tokens, ErrorReporterPtr errorReporter)
      : tokens(tokens), errorReporter(errorReporter) {}
  Expr *parse();

private:
  int current = 0;
  vector<Token> tokens;
  ErrorReporterPtr errorReporter;

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
  ParserError error(Token *token, string message);

  void consume(TokenType type, Token *startingToken, string message);
  void advance();
  void synchronize();
};
