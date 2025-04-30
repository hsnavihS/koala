#pragma once

#include <fstream>
#include <iostream>

using namespace std;

class Logger {
public:
  Logger(const string &logPath) {
    this->debug_log = ofstream(logPath, ios::app);
  };
  ~Logger() {};

  // loggers for info, debug, warning and error
  void info(const string &message) {
    debug_log << "[" << _time() << " KoalaLSP::INFO] " << message << endl;
  }
  void debug(const string &message) {
    debug_log << "[" << _time() << " KoalaLSP::DEBUG] " << message << endl;
  }
  void warn(const string &message) {
    debug_log << "[" << _time() << " KoalaLSP::WARN] " << message << endl;
  }
  void err(const string &message) {
    debug_log << "[" << _time() << " KoalaLSP::ERROR] " << message << endl;
  }

private:
  ofstream debug_log;

  string _time() {
    time_t curr_time;
    curr_time = time(NULL);

    tm *tm_local = localtime(&curr_time);

    return to_string(tm_local->tm_mday) + "/" +
           to_string(tm_local->tm_mon + 1) + " " +
           to_string(tm_local->tm_hour) + ":" + to_string(tm_local->tm_min);
  }
};
