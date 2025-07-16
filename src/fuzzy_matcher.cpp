#include "fuzzy_matcher.hpp"
#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <vector>

constexpr int BASE_SCORE = 1000;
constexpr int CONSECUTIVE_BONUS = 20;
constexpr int WORD_BOUNDARY_BONUS = 30;
constexpr double WORD_BOUNDARY_MULTIPLIER = 2;
constexpr int COMPACTNESS_PENALTY = 30;
constexpr int LATE_MATCH_PENALTY = 50;

bool is_word_boundary(const std::string_view s, size_t i) {
    if (i == 0)
        return true;
    char prev = s[i - 1];
    char curr = s[i];
    return !std::isalnum(prev) || (std::islower(prev) && std::isupper(curr));
}

MatchResult calculate_match(const std::string_view query, const std::string_view line) {
    std::vector<size_t> match_indices;

    match_indices.reserve(query.size()); // stop dynamic growing

    int score = 0;
    int consecutive_match_count = 0;
    int max_consecutive = 0;
    int word_boundary_bonus = 0;
    int first_match_pos = -1;
    int last_match_pos = -1;

    // string preprocesssing to lowercase
    std::string query_lower(query);
    std::string line_lower(line);
    std::transform(query_lower.begin(), query_lower.end(), query_lower.begin(), ::tolower);
    std::transform(line_lower.begin(), line_lower.end(), line_lower.begin(), ::tolower);

    size_t query_pos = 0;
    size_t line_pos = 0;

    while (query_pos < query_lower.size() && line_pos < line_lower.size()) {
        // no space left in rest of query
        if (line_lower.size() - line_pos < query_lower.size() - query_pos)
            break;
        if (query_lower[query_pos] == line_lower[line_pos]) {
            match_indices.push_back(line_pos);
            if (first_match_pos == -1) {
                first_match_pos = static_cast<int>(line_pos);
            }
            last_match_pos = static_cast<int>(line_pos);

            // boundary bonus for variables
            if (is_word_boundary(line, line_pos)) {
                word_boundary_bonus += WORD_BOUNDARY_BONUS;
            }
            // consecutive match bonus
            if (!match_indices.empty() && match_indices.size() >= 2 &&
                match_indices[match_indices.size() - 1] ==
                    match_indices[match_indices.size() - 2] + 1) {
                consecutive_match_count++;
            } else {
                consecutive_match_count = 1;
            }

            max_consecutive = std::max(max_consecutive, consecutive_match_count);

            query_pos++;
        }

        line_pos++;
    }

    // no match then
    if (query_pos < query_lower.size()) {
        return MatchResult{std::string(line), -1, {}};
    }

    int match_span = static_cast<int>(match_indices.back() - match_indices.front() + 1);

    int compactness_penalty = (match_span - static_cast<int>(query.size())) * COMPACTNESS_PENALTY;
    int late_match_penalty = first_match_pos * LATE_MATCH_PENALTY;

    score += max_consecutive * CONSECUTIVE_BONUS;
    score += word_boundary_bonus * WORD_BOUNDARY_MULTIPLIER;
    score -= compactness_penalty;
    score -= late_match_penalty;

    return MatchResult{std::string(line), BASE_SCORE + score, match_indices};
}
std::vector<MatchResult> fuzzy_match(const std::string_view query,
                                     const std::vector<std::string_view>& entries) {
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
