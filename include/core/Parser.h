#pragma once

#include <vector>

#include "Expr.h"
#include "Token.h"
#include "TokenType.h"
#include "core/Stmt.h"
#include "error/ErrorReporter.h"
#include "error/ParserError.h"

class Parser {
public:
  Parser(vector<Token> tokens, ErrorReporterPtr errorReporter)
      : tokens(tokens), errorReporter(errorReporter) {}
  vector<Stmt *> *parse();

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
  Expr *logicalOr();
  Expr *logicalAnd();
  Expr *expression();
  Expr *equality();
  Expr *comparison();
  Expr *term();
  Expr *factor();
  Expr *unary();
  Expr *primary();
  Expr *assignment();

  Stmt *statement();
  Stmt *declaration();
  Stmt *varDeclaration();
  Stmt *printStatement();
  Stmt *expressionStatement();
  Stmt *ifStatement();
  Stmt *whileStatement();
  Stmt *forStatement();

  vector<Stmt *> *block();

  bool check(TokenType type);
  bool match(std::initializer_list<TokenType> types);
  Token *consume(TokenType type, Token *startingToken, string message);

  bool isAtEnd();
  Token *advance();
  Token *previous();

  void synchronize();
  ParserError error(Token *token, string message);
};
