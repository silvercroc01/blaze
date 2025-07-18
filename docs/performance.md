# Blaze Performance Profiling

## Environment
- Date: 2025-07-17
- Machine: [Ryzen 5 6600H, 16GB, Arch Linux]
- Compiler: [g++, (GCC) 15.1.1 20250425]
- Build: -O2 -pg

---

## Dataset 1: english.txt
- Source: [english.txt](https://raw.githubusercontent.com/kkrypt0nn/wordlists/refs/heads/main/wordlists/languages/english.txt)
- Word Count: 354,296
- Query: `od`
- Total Runtime: [0.101s]
```bash
$  time ./src/blaze_main --file ../english.txt --query od >  /dev/null 2>&1
0.06s user 0.04s system 99% cpu 0.101 total
```
### Top Bottlenecks (gprof)
| Function           | % Time | Notes                |
|--------------------|--------|----------------------|
| calculate_match    | 40%    | Matching logic       |
| std::swap (sorting)| 20%    | Sorting results      |
| File reading       | 20%    | I/O bottleneck       |
| Others             | 20%    | Miscellaneous        |

**Notes:**  
- Matching was the main bottleneck before I/O optimization.

---

## Dataset 2: biglist.txt (After mmap Optimization)
- Source: [biglist.txt](https://github.com/kkrypt0nn/wordlists/raw/refs/heads/main/wordlists/passwords/most_used_passwords.txt)
- Word Count: 999,997
- Query: `od`
- Total Runtime: [0.137s]
```bash 
$ time ./src/blaze_main --file ../biglist.txt --query od >  /dev/null 2>&1
0.10s user 0.03s system 99% cpu 0.137 total
 ```


### Top Bottlenecks (gprof)
| Function           | % Time | Notes                |
|--------------------|--------|----------------------|
| calculate_match    | 33%    | Matching logic       |
| std::swap (sorting)| 33%    | Sorting results      |
| File reading (mmap)| 33%    | Now as fast as others|
| Others             | 1%     | Miscellaneous        |

**Notes:**  
- After switching to mmap, file reading is no longer a bottleneck.
- The main processes are now balanced.

---


---

## Dataset 3: biglist.txt (Parallel Matching)

- Source: [biglist.txt](https://github.com/kkrypt0nn/wordlists/raw/refs/heads/main/wordlists/passwords/most_used_passwords.txt)
- Word Count: 999,997
- Query: `od`
- Total Runtime: [0.095s]
```bash 

$ time ./src/blaze_main --file ../biglist.txt --query od >  /dev/null 2>&1
0.23s user 0.03s system 271% cpu 0.095 total
```
### Top Bottlenecks (gprof)
| Function           | % Time | Notes                |
|--------------------|--------|----------------------|
| calculate_match    | 40%    | Matching logic       |
| thread management  | 20%    | Thread management    |
| File reading (mmap)| 20%    | Fast as others       |
| __int              | 20%    | Startup              |
| others             | ~0%    | Miscellaneous

**Notes:**  
- Used threads for parallel matching, now matching is as fast as others.

---
## Summary Table

| Dataset      | File Reading | Matching | Sorting| Threading | Total Time |
|--------------|-------------|----------|---------|-----------|------------|
| english.txt  | 20%         | 40%      | 20%     |   --      | [0.12s]    |
| biglist.txt  | 33%         | 33%      | 33%     |   --      | [0.13s]    |
| biglist.txt  | 20%         | 20%      | --      |   20%     | [0.09s]    |
  
---

## How to Reproduce

1. Build with `-pg` for gprof.
2. Run:
   ```sh
   $ ./src/blaze_main --file [dataset] --query od
   gprof ./src/blaze_main gmon.out > profile.txt
   ```
3. Analyze `profile.txt` for bottlenecks.

---

## Change Log

- **2025-07-17:** Switched to mmap for file reading. Balanced main bottlenecks.
- **2025-07-19:** Implemented parallel matching. Achieved balanced CPU utilization.
