#pragma once

#include <string_view>
#include <vector>
#include "types.hpp"

std::vector<MatchResult> fuzzy_match(const std::string_view query,
                                     const std::vector<std::string_view>& entries);

MatchResult calculate_match(const std::string_view query, const std::string_view line);

// bool is_word_boundary(const std::string_view s, size_t i);
