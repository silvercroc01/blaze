#include <cstddef>
#include <string>
#include <vector>
struct MatchResult {
    std::string line;
    int score;
    std::vector<size_t> match_indices;
};
