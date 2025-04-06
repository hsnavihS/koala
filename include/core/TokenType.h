#pragma once

#include <string>

enum class TokenType {
  // single character tokens
  LEFT_PARENTHESIS,
  RIGHT_PARENTHESIS,
  LEFT_BRACE,
  RIGHT_BRACE,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // tokens that may be one or two characters
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // literals
  IDENTIFIER,
  STRING,
  NUMBER,

  // keywords
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUNC,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  // end of file
  END_OF_FILE
};

// NOTE: Not the most elegant way to do this, but it works for debugging
const std::string tokenTypeToString[] = {
  "LEFT_PARENTHESIS",
  "RIGHT_PARENTHESIS",
  "LEFT_BRACE",
  "RIGHT_BRACE",
  "LEFT_BRACKET",
  "RIGHT_BRACKET",
  "COMMA",
  "DOT",
  "MINUS",
  "PLUS",
  "SEMICOLON",
  "SLASH",
  "STAR",
  "BANG",
  "BANG_EQUAL",
  "EQUAL",
  "EQUAL_EQUAL",
  "GREATER",
  "GREATER_EQUAL",
  "LESS",
  "LESS_EQUAL",
  "IDENTIFIER",
  "STRING",
  "NUMBER",
  "AND",
  "CLASS",
  "ELSE",
  "FALSE",
  "FUNC",
  "FOR",
  "IF",
  "NIL",
  "OR",
  "PRINT",
  "RETURN",
  "SUPER",
  "THIS",
  "TRUE",
  "VAR",
  "WHILE",
  "END_OF_FILE"
};
