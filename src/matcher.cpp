#include "matcher.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

int calculate_score(const std::string& query, const std::string& line) {
  std::cerr << "[DEBUG] Calculating score: query = '" << query << "', line = '" << line << "'\n";
  int q_len = query.size();
  int l_len = line.size();

  int q_idx = 0, l_idx = 0;

  int first_match = -1;
  int last_match = -1;

  while (q_idx < q_len && l_idx < l_len) {
    if (tolower(query[q_idx]) == tolower(line[l_idx])) {
      // closer char gets more score
      if (first_match == -1) first_match = l_idx;
      last_match = l_idx;
      ++q_idx;
    }
    ++l_idx;
  }
  // no full match
  if (q_idx != q_len) {
    std::cerr << "[DEBUG] No match\n";
    return -1;
  }
  int match_span = last_match - first_match + 1;

  int gaps = match_span - q_len;

  int score = 100;
  // better early match
  score -= first_match * 4;
  // less gaps
  score -= gaps * 8;

  // prefix bonus
  if (first_match == 0) score += 25;
  std::cerr << "[DEBUG] Score = " << score << "\n";
  return score;
}

std::vector<MatchResult> fuzzy_match(const std::string& query,
                                     const std::vector<std::string>& entries) {
  std::cerr << "[DEBUG] Entered fuzzy_match()\n";
  std::vector<MatchResult> results;
  for (const auto& line : entries) {
    std::cerr << "[DEBUG] Matching line: " << line << "\n";
    int score = calculate_score(query, line);

    std::cerr << "[DEBUG] Score for : " << line << " : " << score << "\n";
    if (score >= 0) {
      results.push_back({line, score});

      std::cerr << "[DEBUG] line: " << line << ", score: " << score << "\n";
    }
  }
  std::sort(results.begin(), results.end(),
            [](const MatchResult& a, const MatchResult& b) { return a.score > b.score; });
  return results;
}
