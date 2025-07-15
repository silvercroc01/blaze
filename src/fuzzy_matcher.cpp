#include "fuzzy_matcher.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
MatchResult calculate_match(const std::string& query, const std::string& line) {
    std::vector<size_t> indices;

    indices.reserve(query.size()); // stop dynamic growing

    // string preprocesssing to lowercase
    std::string q_lower = query, l_lower = line;
    std::transform(q_lower.begin(), q_lower.end(), q_lower.begin(), ::tolower);
    std::transform(l_lower.begin(), l_lower.end(), l_lower.begin(), ::tolower);

    size_t q_idx = 0;
    size_t l_idx = 0;

    int score = 0;
    int last_match = -1;

    while (q_idx < query.size() && l_idx < line.size()) {
        // no space left in rest of query
        if (l_lower.size() - l_idx < q_lower.size() - q_idx)
            break;
        if (q_lower[q_idx] == l_lower[l_idx]) {
            indices.push_back(l_idx);

            if (last_match != -1) {
                int gap = (int)(l_idx - last_match); // higher penalty for gaps
                score += gap * gap;
            } else {
                score += (int)l_idx * 2; // penalize  late first match
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

    int start_bonus = 0;
    if (!indices.empty()) {
        if (indices.front() == 0) {
            start_bonus += 50;
        } else if (indices.front() < 3) {
            start_bonus += 25;
        }

        // bonus for better variable searching

        else if (indices.front() > 0 &&
                 (isspace(line[indices.front() - 1]) || line[indices.front() - 1] == '_')) {
            start_bonus += 30;
        }
        if (isupper(line[indices.front()]) && indices.front() > 0 &&
            islower(line[indices.front() - 1])) {
            start_bonus += 20;
        }
    }

    int contiguous_bonus = 0;
    int run = 1;
    for (size_t i = 1; i < indices.size(); i++) {
        if (indices[i] == indices[i - 1] + 1) {
            ++run;
        } else {
            if (run > 1) {
                contiguous_bonus += run * run;
                run = 1;
            }
        }
    }
    if (run > 1)
        contiguous_bonus += run * run;

    int match_span = (int)(indices.back() - indices.front() + 1);

    int compact_bonus = (int)(40.0 * query.size() / match_span); // normalised for words

    int position_penalty = (int)(indices.front()) * 4;

    score += position_penalty;

    score += (match_span - query.size()) * 4; // for large match spans

    int final_score = 1000 - score + start_bonus + contiguous_bonus + compact_bonus;

    std::cerr << "[DEBUG] Line: " << line << ", Score: " << score << ", StartBonus: " << start_bonus
              << ", ContigBonus: " << contiguous_bonus << ", CompactBonus: " << compact_bonus
              << ", FinalScore: " << final_score << ", MatchSpan: " << match_span
              << ", PosPenalty: " << position_penalty << ", Indices: ";

    for (auto i : indices)
        std::cerr << i << " ";
    std::cerr << "\n";

    return MatchResult{line, final_score, indices};
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
