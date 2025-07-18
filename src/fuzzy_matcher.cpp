#include "fuzzy_matcher.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <future>
#include <iterator>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

constexpr int BASE_SCORE = 1000;
constexpr int CONSECUTIVE_BONUS = 20;
constexpr int WORD_BOUNDARY_BONUS = 30;
constexpr int WORD_BOUNDARY_MULTIPLIER = 2;
constexpr int COMPACTNESS_PENALTY = 30;
constexpr int LATE_MATCH_PENALTY = 50;

inline bool is_word_boundary(const std::string_view s, size_t i) {
    if (i == 0)
        return true;
    char prev = s[i - 1];
    char curr = s[i];
    return !std::isalnum(prev) || (std::islower(prev) && std::isupper(curr));
}

inline bool is_char_equal(char a, char b) {
    return std::tolower(static_cast<unsigned char>(a)) ==
           std::tolower(static_cast<unsigned char>(b));
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

    size_t query_pos = 0;
    size_t line_pos = 0;

    while (query_pos < query.size() && line_pos < line.size()) {
        // no space left in rest of query
        if (line.size() - line_pos < query.size() - query_pos)
            break;
        if (is_char_equal(query[query_pos], line[line_pos])) {
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
    if (query_pos < query.size()) {
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

    size_t num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) {
        num_threads = 4; // fallback to a default value
    }

    // efficient thread utilization
    size_t chunk_size = (entries.size() + num_threads - 1) / num_threads;
    std::vector<std::future<std::vector<MatchResult>>> futures;

    for (size_t t = 0; t < num_threads; ++t) {
        size_t start = t * chunk_size;
        size_t end = std::min(start + chunk_size, entries.size());

        futures.push_back(std::async(std::launch::async, [&, start, end]() {
            std::vector<MatchResult> local_results;
            for (size_t i = start; i < end; ++i) {
                auto result = calculate_match(query, entries[i]);
                if (result.score >= 0) {
                    local_results.push_back(result);
                }
            }
            return local_results;
        }));
    }

    std::vector<MatchResult> results;

    size_t total_size = 0;
    // reserve size

    results.reserve(entries.size());

    for (auto& fut : futures) {
        auto local = fut.get();
        results.insert(results.end(), std::make_move_iterator(local.begin()),
                       std::make_move_iterator(local.end()));
    }
    std::sort(results.begin(), results.end(),
              [](const MatchResult& a, const MatchResult& b) { return a.score > b.score; });
    return results;
}
