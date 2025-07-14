
#pragma once

#include <string>
#include <vector>

struct MatchResult {
  std::string line;
  int score;
};

std::vector<MatchResult> fuzzy_match(const std::string& query,
                                     const std::vector<std::string>& entries);

std::string highlight_match(const std::string& line, const std::string& query);
