#include <unordered_map>

#include "../include/RequestHandler.hpp"

using namespace std;

RequestHandler::RequestHandler(Logger *logger, Responder *responder)
    : logger(logger), responder(responder) {
  this->dispatch_table = {
      {"initialize", std::bind(&RequestHandler::handleInitialize, this,
                               std::placeholders::_1)},
      {"shutdown",
       std::bind(&RequestHandler::handleShutdown, this, std::placeholders::_1)},
      {"exit",
       std::bind(&RequestHandler::handleExit, this, std::placeholders::_1)},
  };
}

void RequestHandler::handle(const LspMessage &msg) {
  if (this->shutdownReceived) {
    logger->info("Shutdown already received, ignoring message");
    return;
  }

  auto it = dispatch_table.find(msg.method);
  if (it != dispatch_table.end()) {
    it->second(msg);
  } else {
    logger->err("Unhandled method: " + msg.method);
  }
}

void RequestHandler::handleInitialize(const LspMessage &message) {
  json initializeResult = {
      {"capabilities",
       {
           {"textDocumentSync", 1},
           {"hoverProvider", false},
           {"definitionProvider", false},
       }},
      {"serverInfo",
       {
           {"name", "Koala LSP"},
           {"version", "0.0.1"},
       }},
  };
  responder->sendResult(message.id, initializeResult);
}

void RequestHandler::handleInitialized(const LspMessage &message) {
  logger->info("Initialized");
}

void RequestHandler::handleShutdown(const LspMessage &message) {
  this->shutdownReceived = true;
  responder->sendResult(message.id, nullptr);
}

void RequestHandler::handleExit(const LspMessage &message) {
  if (this->shutdownReceived) {
    logger->info("Shutting down server");
    exit(0);
  } else {
    logger->err("Exit received without shutdown");
    exit(1);
  }
}
