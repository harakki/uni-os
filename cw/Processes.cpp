#include "Processes.h"

#include <pwd.h>
#include <unistd.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;

string Processes::getUserName(uid_t uid) {
  struct passwd pwd;
  struct passwd* result;
  char buf[256];

  return (getpwuid_r(uid, &pwd, buf, sizeof(buf), &result) == 0 && result)
             ? string(pwd.pw_name)
             : std::to_string(uid);
}

void Processes::updateSystemUptime() {
  std::ifstream uptime_file("/proc/uptime");
  if (!uptime_file.is_open())
    throw std::runtime_error("Failed to open /proc/uptime");

  string line;

  std::getline(uptime_file, line);
  std::istringstream iss(line);
  iss >> system_uptime;
}

void Processes::updateTotalMemory() {
  std::ifstream meminfo_file("/proc/meminfo");
  if (!meminfo_file.is_open())
    throw std::runtime_error("Failed to open /proc/meminfo");

  string line;

  while (std::getline(meminfo_file, line)) {
    if (line.find("MemTotal:") == 0) {
      std::istringstream iss(line.substr(10));
      iss >> total_memory;
    }
  }
}

void Processes::parseStat(const string& pid_dir, ProcessInfo* proc) {
  std::ifstream stat_file("/proc/" + pid_dir + "/stat");
  if (!stat_file.is_open()) {
    throw std::runtime_error("Failed to open /proc/" + pid_dir + "/stat");
  }
  string line;

  std::getline(stat_file, line);
  std::istringstream iss(line);
  iss >> /*1*/ proc->pid >> /*2*/ proc->comm >> /*3*/ proc->state >>
      /*4*/ proc->ppid >> /*5*/ proc->pgrp >> /*6*/ proc->session >>
      /*7*/ proc->tty_nr >> /*8*/ proc->tpgid >> /*9*/ std::skipws >>
      /*10*/ std::skipws >> /*11*/ std::skipws >> /*12*/ std::skipws >>
      /*13*/ std::skipws >> /*14*/ proc->utime >> /*15*/ proc->stime >>
      /*16*/ proc->cutime >> /*17*/ proc->cstime >> /*18*/ proc->priority >>
      /*19*/ proc->nice >> /*20*/ proc->num_threads >> /*21*/ std::skipws >>
      /*22*/ proc->starttime >> /*23*/ proc->vsize >> /*24*/ proc->rss;

  const long clock_ticks = sysconf(_SC_CLK_TCK);
  proc->utime /= clock_ticks;
  proc->stime /= clock_ticks;
  proc->starttime /= clock_ticks;
}

void Processes::parseStatus(const string& pid_dir, ProcessInfo* proc) {
  std::ifstream status_file("/proc/" + pid_dir + "/status");
  if (!status_file.is_open())
    throw std::runtime_error("Failed to open /proc/" + pid_dir + "/status");

  string line;

  while (std::getline(status_file, line)) {
    if (line.find("Uid:") == 0) {
      std::istringstream iss(line.substr(5));
      iss >> proc->uid;
      proc->user_name = getUserName(proc->uid);
    }

    if (line.find("VmRSS:") == 0) {
      std::istringstream iss(line.substr(7));
      iss >> proc->vmrss;
    }
  }
}

void Processes::parseCmdline(const string& pid_dir, ProcessInfo* proc) {
  // Binary read because '\0' is a variables delimeter
  std::ifstream cmdline_file("/proc/" + pid_dir + "/cmdline", std::ios::binary);
  if (!cmdline_file.is_open())
    throw std::runtime_error("Failed to open /proc/" + pid_dir + "/cmdline");

  string cmdline((std::istreambuf_iterator<char>(cmdline_file)),
                 std::istreambuf_iterator<char>());
  std::replace(cmdline.begin(), cmdline.end(), '\0', ' ');
  proc->cmdline = cmdline;
}

ProcessInfo Processes::parseProcess(const string& pid_dir) {
  ProcessInfo proc;

  parseStat(pid_dir, &proc);
  parseStatus(pid_dir, &proc);
  parseCmdline(pid_dir, &proc);

  return proc;
}

std::vector<ProcessInfo> Processes::getInfo() {
  std::vector<ProcessInfo> processes;

  updateSystemUptime();
  updateTotalMemory();

  fs::path proc_path("/proc");
  if (!fs::exists(proc_path) || !fs::is_directory(proc_path))
    throw std::runtime_error("/proc is not accessible");

  for (const auto& entry : fs::directory_iterator(proc_path)) {
    if (entry.is_directory()) {
      const std::string filename = entry.path().filename().string();
      if (std::all_of(filename.begin(), filename.end(), ::isdigit)) {
        try {
          processes.push_back(parseProcess(filename));
        } catch (std::exception& ex) {
          continue;
        }
      }
    }
  }

  return processes;
}
