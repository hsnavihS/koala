#include <string>
#include <vector>

#include "core/Lexer.h"
#include "types/Token.h"

using namespace std;

vector<Token> Lexer::generateTokens() {
  vector<Token> tokens;

  for (int i = 0; i < code.size(); i++) {
    switch (code[i]) {
    // single character tokens
    case '(':
      tokens.push_back(
          Token(TokenType::LEFT_PARENTHESIS, "(", "", line, ++column));
      continue;
    case ')':
      tokens.push_back(
          Token(TokenType::RIGHT_PARENTHESIS, ")", "", line, ++column));
      continue;
    case '{':
      tokens.push_back(Token(TokenType::LEFT_BRACE, "{", "", line, ++column));
      continue;
    case '}':
      tokens.push_back(Token(TokenType::RIGHT_BRACE, "}", "", line, ++column));
      continue;
    case '[':
      tokens.push_back(Token(TokenType::LEFT_BRACKET, "[", "", line, ++column));
      continue;
    case ']':
      tokens.push_back(
          Token(TokenType::RIGHT_BRACKET, "]", "", line, ++column));
      continue;
    case ',':
      tokens.push_back(Token(TokenType::COMMA, ",", "", line, ++column));
      continue;
    case '.':
      tokens.push_back(Token(TokenType::DOT, ".", "", line, ++column));
      continue;
    case '-':
      tokens.push_back(Token(TokenType::MINUS, "-", "", line, ++column));
      continue;
    case '+':
      tokens.push_back(Token(TokenType::PLUS, "+", "", line, ++column));
      continue;
    case ';':
      tokens.push_back(Token(TokenType::SEMICOLON, ";", "", line, ++column));
      continue;
    // TODO: Handle comments
    case '/':
      tokens.push_back(Token(TokenType::SLASH, "/", "", line, ++column));
      continue;
    case '*':
      tokens.push_back(Token(TokenType::STAR, "*", "", line, ++column));
      continue;

    // tokens that may be two characters long
    case '!':
      if (peek(i) == '=') {
        tokens.push_back(Token(TokenType::BANG_EQUAL, "!=", "", line, column));
        i++;
        column += 2;
      } else {
        tokens.push_back(Token(TokenType::BANG, "!", "", line, ++column));
      }
      continue;
    case '=':
      if (peek(i) == '=') {
        tokens.push_back(Token(TokenType::EQUAL_EQUAL, "==", "", line, column));
        i++;
        column += 2;
      } else {
        tokens.push_back(Token(TokenType::EQUAL, "=", "", line, ++column));
      }
      continue;
    case '>':
      if (peek(i) == '=') {
        tokens.push_back(
            Token(TokenType::GREATER_EQUAL, ">=", "", line, column));
        i++;
        column += 2;
      } else {
        tokens.push_back(Token(TokenType::GREATER, ">", "", line, ++column));
      }
      continue;
    case '<':
      if (peek(i) == '=') {
        tokens.push_back(Token(TokenType::LESS_EQUAL, "<=", "", line, column));
        i++;
        column += 2;
      } else {
        tokens.push_back(Token(TokenType::LESS, "<", "", line, ++column));
      }
      continue;

      // whitespaces
    case '\n':
      if (i != code.size() - 1) {
        line++;
        column = 0;
      }
      continue;
    case '\r':
      continue;
    case '\t':
      column += 4; // TODO: Handle tabs
      continue;
    case ' ':
      column++;
      continue;

    // handle strings, literals, numbers, identifiers and errors
    default:
      if (code[i] == '"') {
        tokens.push_back(processString(&i));
        i--;
      } else if (isalpha(code[i]) || code[i] == '_') {
        tokens.push_back(processIdentifier(&i));
        i--;
      } else if (isdigit(code[i])) {
        tokens.push_back(processNumber(&i));
        i--;
      } else {
        errorReporter->report(
            line, column, "Unexpected character: '" + string(1, code[i]) + "'");
      }
    }
  }

  tokens.push_back(
      Token(TokenType::END_OF_FILE, "EOF", nullptr, line, column + 1));
  return tokens;
}

Token Lexer::processNumber(int *i) {
  string buf = "";
  while (isdigit(code[*i])) {
    buf += code[*i];
    *i = *i + 1;
  }
  column += buf.size();
  return Token(TokenType::NUMBER, buf, stoi(buf), line, column);
}

Token Lexer::processString(int *i) {
  string buf = "";
  *i = *i + 1; // skip the opening quote

  while (code[*i] != '"') {
    buf += code[*i];
    *i = *i + 1;
  }

  // TODO: Handle unterminated strings

  *i = *i + 1;              // skip the closing quote
  column += buf.size() + 2; // +2 for the quotes
  return Token(TokenType::STRING, buf, buf, line, column);
}

Token Lexer::processIdentifier(int *i) {
  string buf = "";
  buf.push_back(code[*i]);
  *i = *i + 1;

  while (isalnum(code[*i]) || code[*i] == '_') {
    buf += code[*i];
    *i = *i + 1;
  }

  bool isKeyword = keywords.find(buf) != keywords.end();

  column += buf.size();
  if (isKeyword) {
    return Token(keywords[buf], buf, "", line, column);
  } else {
    return Token(TokenType::IDENTIFIER, buf, "", line, column);
  }
}

char Lexer::peek(int i) {
  if (i < code.size()) {
    return code[i + 1];
  } else {
    return '\0';
  }
}
