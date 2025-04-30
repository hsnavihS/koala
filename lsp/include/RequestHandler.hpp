#pragma once

#include <string>
#include <unordered_map>

#include "Logger.hpp"
#include "Responder.hpp"
#include "types/LspMessage.hpp"

using namespace std;

class RequestHandler {
public:
  RequestHandler(Logger *logger, Responder *responder);
  void handle(const LspMessage &msg);

private:
  bool shutdownReceived = false;

  Logger *logger;
  Responder *responder;

  std::unordered_map<std::string, std::function<void(const LspMessage &)>>
      dispatch_table;

  void handleInitialize(const LspMessage &msg);
  void handleInitialized(const LspMessage &msg);
  void handleShutdown(const LspMessage &msg);
  void handleExit(const LspMessage &msg);
};
