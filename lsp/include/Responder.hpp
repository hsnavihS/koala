#pragma once

#include "Logger.hpp"
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

class Responder {
public:
  Responder(Logger *logger) : logger(logger) {}
  ~Responder() {}

  void sendResult(const int &id, const json &result);
  void sendError(const int &id, const json &error);
  void sendNotification(const string &method, const json &params);

private:
  Logger *logger;
};
