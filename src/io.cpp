#include <cstddef>
#include <cstdio>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

// mmap for fast file i/o

std::vector<std::string_view> read_lines_mmap(const std::string& file_path,
                                              std::string& file_data) {
    std::vector<std::string_view> lines;
    int fd = open(file_path.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("open");
        return lines;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        close(fd);
        return lines;
    }
    if (sb.st_size == 0) {
        close(fd);
        return lines;
    }

    // mapping in memory
    char* data = (char*)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return lines;
    }

    // copy mapped dat to string
    file_data.assign(data, sb.st_size);

    // unmap and close file
    munmap(data, sb.st_size);
    close(fd);

    // split file_data into line in string_view for fast search

    size_t start = 0;

    for (size_t i = 0; i < file_data.size(); i++) {
        if (file_data[i] == '\n') {
            lines.emplace_back(&file_data[start], i - start);
            start = i + 1;
        }
    }

    if (start < file_data.size()) {
        lines.emplace_back(&file_data[start], file_data.size() - start);
    }
    return lines;
}

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
