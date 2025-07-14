#include <iostream>
#include <string>
#include <vector>
#include "matcher.hpp"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " search_query>\n";
    return 1;
  }
  std::string query = argv[1];
  std::vector<std::string> entries;
  std::string line;
  while (std::getline(std ::cin, line)) {
    entries.push_back(line);
  }
  auto results = fuzzy_match(query, entries);

  for (const auto& result : results) {
    std::string highlighted = highlight_match(result.line, query);
    std::cout << highlighted << "\n";
  }
  return 0;
}
