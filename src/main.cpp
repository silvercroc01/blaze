#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include "config.hpp"
#include "fuzzy_matcher.hpp"
#include "highlighter.hpp"
#include "io.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <search_query>\n";
        return 1;
    }
    Config config = parse_args(argc, argv);

    if (config.query.empty()) {
        std::cerr << "Usage: blaze [--query <str>] [--limit N] [--file FILE] [--no-color]\n";
        return 1;
    }

    std::cerr << "[DEBUG] Query: " << config.query << "\n";

    std::vector<std::string_view> entries;
    std::string file_data;

    if (!config.file_path.empty()) {
        std::cerr << "[DEBUG] Opening File: " << config.file_path << "\n";
        entries = read_lines_mmap(config.file_path, file_data);
    } else {
        std::cerr << "[DEBUG] reading from stdin: \n";
        std::string line;
        while (std::getline(std::cin, line)) {
            entries.push_back(line);
        }
    }

    std::cerr << "[DEBUG] Read " << entries.size() << " lines\n";

    // std::string line;
    // while (std::getline(std ::cin, line)) {
    //   entries.push_back(line);
    // }

    auto results = fuzzy_match(config.query, entries);
    std::cerr << "[DEBUG] Got " << results.size() << " matched lines\n";

    // arg: limit
    if (config.limit > 0 && results.size() > (size_t)config.limit) {
        results.resize(config.limit);
    }

    for (const auto& result : results) {
        std::cerr << "[DEBUG] Output: " << result.line << " (score: " << result.score << ")\n";

        std::string output =
            highlight_match(result.line, config.query, result.match_indices, !config.no_color);
        std::cout << output << "\n";
    }
    return 0;
}
