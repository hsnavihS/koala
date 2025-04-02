#include <iostream>
#include <string>
#include <vector>

#include "../../include/Parser.h"
#include "../../include/Token.h"

using namespace std;

vector<Token> Parser::parseTokens() {
  vector<Token> tokens;

  for (int i = 0; i < code.size(); i++) {
    switch (code[i]) {
    // single character tokens
    case '(':
      tokens.push_back(Token(TokenType::LEFT_PARENTHESIS, "(", "", line));
      continue;
    case ')':
      tokens.push_back(Token(TokenType::RIGHT_PARENTHESIS, ")", "", line));
      continue;
    case '{':
      tokens.push_back(Token(TokenType::LEFT_BRACE, "{", "", line));
      continue;
    case '}':
      tokens.push_back(Token(TokenType::RIGHT_BRACE, "}", "", line));
      continue;
    case '[':
      tokens.push_back(Token(TokenType::LEFT_BRACKET, "[", "", line));
      continue;
    case ']':
      tokens.push_back(Token(TokenType::RIGHT_BRACKET, "]", "", line));
      continue;
    case ',':
      tokens.push_back(Token(TokenType::COMMA, ",", "", line));
      continue;
    case '.':
      tokens.push_back(Token(TokenType::DOT, ".", "", line));
      continue;
    case '-':
      tokens.push_back(Token(TokenType::MINUS, "-", "", line));
      continue;
    case '+':
      tokens.push_back(Token(TokenType::PLUS, "+", "", line));
      continue;
    case ';':
      tokens.push_back(Token(TokenType::SEMICOLON, ";", "", line));
      continue;
    // TODO: Handle comments
    case '/':
      tokens.push_back(Token(TokenType::SLASH, "/", "", line));
      continue;
    case '*':
      tokens.push_back(Token(TokenType::STAR, "*", "", line));
      continue;

    // tokens that may be two characters long
    case '!':
      if (peek(i) == '=') {
        tokens.push_back(Token(TokenType::BANG_EQUAL, "!=", "", line));
        i++;
      } else {
        tokens.push_back(Token(TokenType::BANG, "!", "", line));
      }
      continue;
    case '=':
      if (peek(i) == '=') {
        tokens.push_back(Token(TokenType::EQUAL_EQUAL, "==", "", line));
        i++;
      } else {
        tokens.push_back(Token(TokenType::EQUAL, "=", "", line));
      }
      continue;
    case '>':
      if (peek(i) == '=') {
        tokens.push_back(Token(TokenType::GREATER_EQUAL, ">=", "", line));
        i++;
      } else {
        tokens.push_back(Token(TokenType::GREATER, ">", "", line));
      }
      continue;
    case '<':
      if (peek(i) == '=') {
        tokens.push_back(Token(TokenType::LESS_EQUAL, "<=", "", line));
        i++;
      } else {
        tokens.push_back(Token(TokenType::LESS, "<", "", line));
      }
      continue;

      // whitespaces
    case '\n':
      line++;
      continue;
    case '\r':
      continue;
    case '\t':
      continue;
    case ' ':
      continue;

    // handle strings, literals, numbers, identifiers and errors
    default:
      if (code[i] == '"') {
        tokens.push_back(parseString(&i));
        i--;
      } else if (isalpha(code[i]) || code[i] == '_') {
        tokens.push_back(parseIdentifier(&i));
        i--;
      } else if (isdigit(code[i])) {
        tokens.push_back(parseNumber(&i));
        i--;
      } else {
        cerr << "Error: Unexpected character '" << code[i] << "' at position "
             << i << endl;
      }
    }
  }

  return tokens;
}

Token Parser::parseNumber(int *i) {
  string buf = "";
  while (isdigit(code[*i])) {
    buf += code[*i];
    *i = *i + 1;
  }
  return Token(TokenType::NUMBER, buf, "", line);
}

Token Parser::parseString(int *i) {
  string buf = "";
  *i = *i + 1; // skip the opening quote

  while (code[*i] != '"') {
    buf += code[*i];
    *i = *i + 1;
  }

  // NOTE: Unterminated strings

  *i = *i + 1; // skip the closing quote
  return Token(TokenType::STRING, buf, "", line);
}

Token Parser::parseIdentifier(int *i) {
  string buf = "";
  buf.push_back(code[*i]);
  *i = *i + 1;

  while (isalnum(code[*i]) || code[*i] == '_') {
    buf += code[*i];
    *i = *i + 1;
  }

  bool isKeyword = keywords.find(buf) != keywords.end();

  if (isKeyword) {
    return Token(keywords[buf], buf, "", line);
  } else {
    return Token(TokenType::IDENTIFIER, buf, "", line);
  }
}

char Parser::peek(int i) {
  if (i < code.size()) {
    return code[i + 1];
  } else {
    return '\0';
  }
}
