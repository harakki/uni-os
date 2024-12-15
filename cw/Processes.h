#pragma once

#include <string>
#include <vector>

#include "ProcessInfo.h"

using std::string;

class Processes {
 private:
  unsigned long system_uptime;
  unsigned long total_memory;

  string getUserName(uid_t uid);

  void updateSystemUptime();
  void updateTotalMemory();

  void parseStat(const string& pid_dir, ProcessInfo* proc);
  void parseStatus(const string& pid_dir, ProcessInfo* proc);
  void parseCmdline(const string& pid_dir, ProcessInfo* proc);

  ProcessInfo parseProcess(const string& pid_dir);

 public:
  std::vector<ProcessInfo> getInfo();
};
