#include "config.hpp"
#include <iostream>
#include <string>

Config parse_args(int argc, char* argv[]) {
  Config config;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--file" && i + 1 < argc) {
      config.file_path = argv[++i];
    } else if (arg == "--query" && i + 1 < argc) {
      config.query = argv[++i];
    } else if (arg == "--limit" && i + 1 < argc) {
      config.limit = std::stoi(argv[++i]);
    } else if (arg == "--no-color") {
      config.no_color = true;
    } else if (arg[0] != '-') {
      if (config.query.empty()) config.query = arg;
    } else {
      std::cerr << "Unknown Argument: " << arg << "\n";
    }
  }
  return config;
}
