#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "../include/Scanner.hpp"

using namespace std;
using json = nlohmann::json;

json parse_json(const string &json_str) {
  json parsed_msg = json::parse(json_str);
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

  // Try to parse the ID only if the message is not a notification
  bool is_notification = !parsed_msg.contains("id");
  int id = -1;
  if (!is_notification && !parsed_msg["id"].is_null()) {
    id = parsed_msg["id"].get<int>();
  }

  return {parsed_msg["method"].get<string>(), id,
          parsed_msg.contains("params") ? parsed_msg["params"] : json::object(),
          parsed_msg, is_notification};
}
