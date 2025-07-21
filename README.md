
# Blaze

**Blaze** is a blazing-fast, modern fuzzy finder for the terminal, written in C++.  
It provides an interactive command-line interface for searching, previewing, and selecting files or strings, inspired by tools like [fzf](https://github.com/junegunn/fzf) and [ripgrep](https://github.com/BurntSushi/ripgrep).

---

## Features

- **Fast fuzzy matching** with multi-threaded search
- **Highlighting** of matched characters
- **Configurable** and extensible
- **Easy to build** with CMake
- Can read from **stdin, files and piped input**
---

## Installation

### Prerequisites

- C++17 or newer compiler (tested with GCC, Clang)
- [CMake](https://cmake.org/) 3.14+

### Build

```bash
git clone https://github.com/silvercroc01/blaze.git
cd blaze
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j
```

## Usage 

```bash 
./src/blaze_main --file /path/to/file --query <search term> --no-color <no colored o/p>
```
