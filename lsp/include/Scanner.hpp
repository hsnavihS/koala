#pragma once

#include "Logger.hpp"
#include "types/LspMessage.hpp"

using namespace std;

class Scanner {
public:
  Scanner(Logger *logger) : logger(logger) {}
  ~Scanner() {}
  LspMessage parseMessage(istream &cin);

private:
  Logger *logger;
};
