#pragma once

#include <string>
#include <vector>
#include "types.hpp"

std::vector<MatchResult> fuzzy_match(const std::string& query,
                                     const std::vector<std::string>& entries);

MatchResult calculate_match(const std::string& query, const std::string& line);

bool is_word_boundary(const std::string& s, size_t i);
