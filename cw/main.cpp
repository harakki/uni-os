#include <ncurses.h>
#include <unistd.h>

#include <atomic>
#include <csignal>
#include <mutex>
#include <thread>

#include "Processes.h"

std::mutex data_mutex;
std::atomic<bool> running(true);

void printProcesses(const std::vector<ProcessInfo>& processes) {
  clear();

  attron(COLOR_PAIR(1));
  mvprintw(0, 0, "%-8s %-8s %-7s %-10s %-*s", "USER", "PID", "STATE",
           "MEM (kB)", COLS - 35, "COMMAND");
  attroff(COLOR_PAIR(1));

  int row = 1;
  for (const auto& proc : processes) {
    if (row >= LINES - 1) break;

    mvprintw(row, 0, "%-8s %-8d %-7s %-10ld %-s",
             proc.user_name.substr(0, 7).c_str(), proc.pid, proc.state.c_str(),
             proc.vmrss, proc.cmdline.c_str());
    row++;
  }

  refresh();
}

void updateProcesses(Processes& processes,
                     std::vector<ProcessInfo>& process_list) {
  while (running) {
    {
      std::lock_guard<std::mutex> lock(data_mutex);
      process_list = processes.getInfo();
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }
}

int main() {
  signal(SIGINT, [](int) {
    endwin();
    exit(0);
  });

  Processes processes;
  std::vector<ProcessInfo> process_list;

  // ncurses init
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);

  std::thread updater(updateProcesses, std::ref(processes),
                      std::ref(process_list));

  while (running) {
    {
      std::lock_guard<std::mutex> lock(data_mutex);
      printProcesses(process_list);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  running = false;
  updater.join();

  endwin();
}
