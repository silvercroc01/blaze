#include <string>
#include <vector>
std::string highlight_match(const std::string& line, const std::string& query,
                           const std::vector<size_t>& indices, bool color = true);
