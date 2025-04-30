#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "include/Logger.hpp"
#include "include/RequestHandler.hpp"
#include "include/Responder.hpp"
#include "include/Scanner.hpp"

using namespace std;
using json = nlohmann::json;

const string logPath = "/tmp/lsp_debug.log";

int main() {
  Logger *logger = new Logger(logPath);
  Scanner *scanner = new Scanner(logger);
  Responder *responder = new Responder(logger);
  RequestHandler *handler = new RequestHandler(logger, responder);
  bool shutdownRequested = false;

  /*
   * The main loop of the LSP server. It reads incoming messages from stdin,
   * parses them, and sends responses back to the client.
   * TODO: Look into how to extend this to accept connections via sockets or
   * other IPC mechanisms.
   */
  while (true) {
    LspMessage message = scanner->parseMessage(cin);
    if (!message.has_value()) {
      logger->err("Empty message, or failed to parse");
      continue;
    }
    handler->handle(message);
  }
}
