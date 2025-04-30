#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "../include/Scanner.hpp"

using namespace std;
using json = nlohmann::json;

json parse_json(const string &json_str) {
  json parsed_msg = json::parse(json_str, nullptr, false);
  if (parsed_msg.is_discarded()) {
    return json();
  }
  return parsed_msg;
}

LspMessage Scanner::parseMessage(istream &cin) {
  string line;
  int content_length = 0;

  // Read content length
  while (getline(cin, line) && !line.empty() && line != "\r") {
    if (line.find("Content-Length:") == 0) {
      content_length = stoi(line.substr(15));
    }
  }

  // Read body
  string body(content_length, '\0');
  cin.read(&body[0], content_length);

  if (body.empty()) {
    return {};
  }

  json parsed_msg = parse_json(body);
  if (parsed_msg.is_null()) {
    return {};
  }

  return {
      parsed_msg["method"],
      parsed_msg["id"],
      parsed_msg["params"],
      parsed_msg,
      parsed_msg["id"].is_null() ? true : false,
  };
}
