#include <unordered_map>

#include "../include/RequestHandler.hpp"

using namespace std;

RequestHandler::RequestHandler(Logger *logger, Responder *responder)
    : logger(logger), responder(responder) {
  this->dispatch_table = {
      {"initialize", std::bind(&RequestHandler::handleInitialize, this,
                               std::placeholders::_1)},
      {"initialized", std::bind(&RequestHandler::handleInitialized, this,
                                std::placeholders::_1)},
      {"shutdown",
       std::bind(&RequestHandler::handleShutdown, this, std::placeholders::_1)},
      {"textDocument/hover",
       std::bind(&RequestHandler::handleHover, this, std::placeholders::_1)},
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
    logger->warn("Unhandled method: " + msg.method);
    // For notifications, we don't need to send an error response
    if (!msg.is_notification) {
      // TODO: Send an error
      return;
    }
  }
}

void RequestHandler::handleInitialize(const LspMessage &message) {
  json initializeResult = {
      {"capabilities",
       {
           {"textDocumentSync", 1},
           {"hoverProvider", true},
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
  responder->sendResult(message.id, json());
}

void RequestHandler::handleHover(const LspMessage &message) {
  logger->debug(message.raw_json.dump());
  json hoverResult = {{"contents",
                       {
                           {"kind", "markdown"},
                           {"value", "Hover information"},
                       }}};
  responder->sendResult(message.id, hoverResult);
}
