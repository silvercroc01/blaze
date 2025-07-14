#pragma once

#include <string>

struct Config {
  std::string query;
  int limit = -1;
  bool no_color = false;
  std::string file_path;
};
Config parse_args(int argc, char* argv[]);
