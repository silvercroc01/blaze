#include <sstream>
#include <string>
#include <vector>
std::string highlight_match(const std::string& line, const std::string& query,
                            const std::vector<size_t>& indices, bool color) {
  if (!color || indices.empty()) return line;
  std::ostringstream out;
  size_t idx = 0;

  std::string highlighted;
  size_t q_idx = 0;

  for (size_t i = 0; i < line.size(); i++) {
    if (idx < indices.size() && i == indices[idx]) {
      out << "\033[1;32m" << line[i] << "\033[0m";
      ++idx;
    } else {
      out << line[i];
    }
  }
  return out.str();
}
