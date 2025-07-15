#include "fuzzy_matcher.hpp"
#include <algorithm>
#include <cctype>
#include <vector>
MatchResult calculate_match(const std::string& query, const std::string& line) {
  std::vector<size_t> indices;

  size_t q_idx = 0;
  size_t l_idx = 0;

  int score = 0;
  int last_match = -1;

  while (q_idx < query.size() && l_idx < line.size()) {
    if (tolower(query[q_idx]) == tolower(line[l_idx])) {
      indices.push_back(l_idx);

      // close matches
      if (last_match != -1) {
        score += (int)(l_idx - last_match);
      } else {
        // penalize first match
        score += (int)l_idx;
      }
      last_match = l_idx;
      ++q_idx;
    }
    ++l_idx;
  }
  // no match then
  if (q_idx < query.size()) {
    return MatchResult{line, -1, {}};
  }

  return MatchResult{line, 1000 - score, indices};
}

std::vector<MatchResult> fuzzy_match(const std::string& query,
                                     const std::vector<std::string>& entries) {
  std::vector<MatchResult> results;
  for (const auto& line : entries) {
    auto result = calculate_match(query, line);
    if (result.score >= 0) {
      results.push_back(result);
    }
  }
  std::sort(results.begin(), results.end(),
            [](const MatchResult& a, const MatchResult& b) { return a.score > b.score; });
  return results;
}
