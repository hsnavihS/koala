#include <ctime>
#include <fstream>
#include <iostream>

#include "../include/Logger.hpp"

using namespace std;

string _time() {
  time_t curr_time;
  curr_time = time(NULL);

  tm *tm_local = localtime(&curr_time);

  return to_string(tm_local->tm_year + 1900) + "-" +
         to_string(tm_local->tm_mon + 1) + "-" + to_string(tm_local->tm_mday) +
         " " + to_string(tm_local->tm_hour) + ":" +
         to_string(tm_local->tm_min) + ":" + to_string(tm_local->tm_sec);
}

void log(const string &message, ofstream &logFile, const string &level) {
  if (logFile.is_open()) {
    logFile << "[" << _time() << "][" << level << "] " << message << endl;
    logFile.flush();
  } else {
    cerr << "Log file is not open" << endl;
  }
}

Logger::Logger(const string &logPath) {
  debug_log.open(logPath, ios::app);
  if (!debug_log.is_open()) {
    cerr << "Failed to open log file: " << logPath << endl;
  }
}

Logger::~Logger() {
  if (debug_log.is_open()) {
    debug_log.close();
  }
}

void Logger::info(const string &message) { log(message, debug_log, "INFO"); }

void Logger::debug(const string &message) { log(message, debug_log, "DEBUG"); }

void Logger::warn(const string &message) { log(message, debug_log, "WARN"); }

void Logger::err(const string &message) { log(message, debug_log, "ERROR"); }
