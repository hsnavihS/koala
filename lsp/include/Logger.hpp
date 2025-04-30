#pragma once

#include <ctime>
#include <fstream>

using namespace std;

class Logger {
public:
  Logger(const string &logPath);
  ~Logger();

  void info(const string &message);
  void debug(const string &message);
  void warn(const string &message);
  void err(const string &message);

private:
  ofstream debug_log;
};
