#include <iostream>
#include <nlohmann/json.hpp>

#include "../include/Responder.hpp"

using namespace std;
using json = nlohmann::json;

void Responder::sendResult(const int &id, const json &result) {
  json response = {
      {"jsonrpc", "2.0"},
      {"id", id},
      {"result", result},
  };
  cout << "Content-Length: " << response.dump().size() << "\r\n\r\n"
       << response.dump();
  cout.flush();
}

void Responder::sendError(const int &id, const json &error) {
  json response = {
      {"jsonrpc", "2.0"},
      {"id", id},
      {"error", error},
  };
  cout << "Content-Length: " << response.dump().size() << "\r\n\r\n"
       << response.dump();
  cout.flush();
}
