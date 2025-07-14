#include "matcher.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

int calculate_score(const std::string& query, const std::string& line) {
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
  if (q_idx != q_len) return -1;
  int match_span = last_match - first_match + 1;
  int gaps = match_span - q_len;

  int score = 100;
  // better early match
  score -= first_match * 4;
  // less gaps
  score -= gaps * 8;

  // prefix bonus
  if (first_match == 0) score += 25;

  return score;
}

std::vector<MatchResult> fuzzy_match(const std::string& query,
                                     const std::vector<std::string>& entries) {
  std::vector<MatchResult> results;
  for (const auto& line : entries) {
    int score = calculate_score(query, line);
    if (score >= 0) {
      results.push_back({line, score});

      std::cerr << "[DEBUG] line: " << line << ", score: " << score << "\n";
    }
  }
  std::sort(results.begin(), results.end(),
            [](const MatchResult& a, const MatchResult& b) { return a.score > b.score; });
  return results;
}

std::string highlight_match(const std::string& line, const std::string& query) {
  std::string result;
  int q_idx = 0;
  int q_len = query.length();

  for (size_t i = 0; i < line.size(); i++) {
    if (q_idx < q_len && std::tolower(line[i]) == std::tolower(query[q_idx])) {
      result += "\033[1;33m";  // Bold Yellow start
      result += line[i];
      result += "\033[0m";  // reset
      ++q_idx;
    } else {
      result += line[i];
    }
  }
  return result;
}
