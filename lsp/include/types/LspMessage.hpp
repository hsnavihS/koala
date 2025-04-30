#pragma once

#include <nlohmann/json.hpp>
#include <string>

using namespace std;
using json = nlohmann::json;

struct LspMessage {
  string method;
  int id = -1;
  json params;
  json raw_json;
  bool is_notification = false;

  bool has_value() {
    return !method.empty() || !params.is_null() || !raw_json.is_null();
  };
};
