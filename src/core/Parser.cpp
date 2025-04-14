#include "core/Parser.h"
#include "error/ParserError.h"
#include "types/Stmt.h"
#include "types/TokenType.h"

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
  if (match({TokenType::LEFT_BRACE})) {
    return new Block(block());
  }
  if (match({TokenType::IF})) {
    return ifStatement();
  }
  if (match({TokenType::WHILE})) {
    return whileStatement();
  }
  if (match({TokenType::FOR})) {
    return forStatement();
  }

  return expressionStatement();
}

Stmt *Parser::whileStatement() {
  consume(TokenType::LEFT_PARENTHESIS, previous(), "Expected '(' after while");
  Expr *condition = expression();
  consume(TokenType::RIGHT_PARENTHESIS, previous(),
          "Expected ')' after while condition");
  Stmt *body = statement();

  return new While(condition, body);
}

Stmt *Parser::forStatement() {
  // Parsing the different parts of the for loop
  consume(TokenType::LEFT_PARENTHESIS, previous(), "Expected '(' after for");

  Stmt *initializer = nullptr;
  if (match({TokenType::SEMICOLON})) {
    initializer = nullptr;
  } else if (match({TokenType::VAR})) {
    initializer = varDeclaration();
  } else {
    initializer = expressionStatement();
  }

  Expr *condition = nullptr;
  if (!check(TokenType::SEMICOLON)) {
    condition = expression();
  }
  consume(TokenType::SEMICOLON, previous(), "Expected ';' after for condition");

  Expr *increment = nullptr;
  if (!check(TokenType::RIGHT_PARENTHESIS)) {
    increment = expression();
  }
  consume(TokenType::RIGHT_PARENTHESIS, previous(),
          "Expected ')' after for increment");

  Stmt *body = statement();

  // Converting the for loop to a while loop
  Block *whileBlock =
      new Block(new vector<Stmt *>({body, new Expression(increment)}));
  condition == nullptr ? new Literal(true) : condition;
  Stmt *whileStmt = new While(condition, whileBlock);
  return initializer == nullptr
             ? whileStmt
             : new Block(new vector<Stmt *>({initializer, whileStmt}));
}

Stmt *Parser::ifStatement() {
  consume(TokenType::LEFT_PARENTHESIS, previous(),
          "Expected '(' after 'if' keyword");
  Expr *condition = expression();
  consume(TokenType::RIGHT_PARENTHESIS, previous(),
          "Expected ')' after 'if' condition");

  Stmt *thenBranch = statement();
  Stmt *elseBranch = nullptr;
  if (match({TokenType::ELSE})) {
    elseBranch = statement();
  }

  return new If(condition, thenBranch, elseBranch);
}

vector<Stmt *> *Parser::block() {
  Token *startingToken = previous();
  vector<Stmt *> *statements = new vector<Stmt *>;

  while (!isAtEnd() && !check(TokenType::RIGHT_BRACE)) {
    statements->push_back(declaration());
  }

  consume(TokenType::RIGHT_BRACE, startingToken,
          "Didn't find matching '}' for this block");
  return statements;
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
  Expr *expr = logicalOr();

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

Expr *Parser::logicalOr() {
  Expr *expr = logicalAnd();

  while (match({TokenType::OR})) {
    Token *op = previous();
    Expr *right = logicalAnd();
    expr = new Logical(expr, op, right);
  }

  return expr;
}

Expr *Parser::logicalAnd() {
  Expr *expr = equality();

  while (match({TokenType::AND})) {
    Token *op = previous();
    Expr *right = equality();
    expr = new Logical(expr, op, right);
  }

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
