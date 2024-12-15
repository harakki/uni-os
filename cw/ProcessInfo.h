#pragma once

#include <string>

struct ProcessInfo {
  /* /proc/{pid}/stat
   * https://man7.org/linux/man-pages/man5/proc_pid_stat.5.html
   */
  int pid;            // (1) Process ID
  std::string comm;   // (2) Filename of executable
  std::string state;  // (3) Process state ([R]unning, [S]leeping, [D]isk sleep,
                      // [Z]ombie, s[T]opped, [X]dead, [I]dle)
  int ppid;           // (4) Parent Process ID
  int pgrp;           // (5) Process Group ID
  int session;        // (6) Session ID of process
  int tty_nr;         // (7) Controlling terminal
  int tpgid;  // (8) ID of foreground process group of controlling terminal
  unsigned long utime;  // (14) User mode CPU time, measured in clock ticks
  unsigned long stime;  // (15) Kernel mode CPU time, measured in clock ticks
  long cutime;  // (16) Time waited-for children in user mode, measured in clock
                // ticks
  long cstime;  // (17) Time waited-for children in kernel mode, measured in
                // clock ticks
  long priority;     // (18) Process priority (higher numbers - lower priority)
  long nice;         // (19) Nice value (used in priority computation)
  long num_threads;  // (20) Number of threads in this process
  unsigned long long starttime;  // (22) Time process started after system boot,
                                 // measured in clock ticks
  unsigned long vsize;           // (23) Virtual memory size in bytes
  long rss;  // (24) Resident Set Size: number of pages in real memory

  /* /proc/{pid}/status */
  int uid;     // User ID
  long vmrss;  // Resident set size on kB. Sum of RssAnon, RssFile, and RssShmem

  /* /proc/{pid}/cmdline */
  std::string cmdline;

  /* OTHER */
  std::string user_name;
};
