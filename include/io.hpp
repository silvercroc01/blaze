#pragma once

#include <string>
#include <string_view>
#include <vector>
std::vector<std::string_view> read_lines_mmap(const std::string& file_path, std::string& file_data);
std::vector<std::string> read_lines(const std::string& file_path);
