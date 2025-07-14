#include <cctype>
#include <string>
std::string highlight_match(const std::string& line, const std::string& query, bool use_color) {
  std::string highlighted;
  size_t q_idx = 0;

  for (char c : line) {
    if (q_idx < query.size() && std::tolower(c) == std::tolower(query[q_idx])) {
      if (use_color) highlighted += "\033[1;32m";
      highlighted += c;
      if (use_color) highlighted += "\033[0m";
      ++q_idx;
    } else {
      highlighted += c;
    }
  }
  return highlighted;
}
