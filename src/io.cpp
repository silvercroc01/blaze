#include <fstream>
#include <iostream>
#include <string>
#include <vector>
std::vector<std::string> read_lines(const std::string& file_path) {
  std::vector<std::string> lines;

  std::istream* input = &std::cin;
  std::ifstream file;

  if (!file_path.empty()) {
    std::cerr << "[DEBUG] Opening file: " << file_path << "\n";
    file.open(file_path);
    if (!file) {
      std::cerr << "Error: could not open file: " << file_path << "\n";
      return lines;
    }
    input = &file;
  }
  std::string line;
  int count = 0;
  while (std::getline(*input, line)) {
    lines.push_back(line);
    if (++count % 10 == 0) {
      std::cerr << "[DEBUG] Read line # " << count << " : " << line << "\n";
    }
  }
  std::cerr << "[DEBUG] Finished reading lines: " << lines.size() << "\n";
  return lines;
}
