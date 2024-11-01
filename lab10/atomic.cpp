#include <array>
#include <atomic>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>

#define THREADS_AMMOUNT 12
#define SEQ_LENGTH 2400000

long g_sum = 0;
std::array<std::atomic<bool>, THREADS_AMMOUNT> flag;
std::atomic<int> turn;

// ======================= Улучшенный алгоритм Петерсона =======================

void enter_critical_section(int id, std::memory_order order_flag,
                            std::memory_order order_turn) {
  int next = (id + 1) % THREADS_AMMOUNT;
  flag[id].store(true, order_flag);
  turn.store(next, order_turn);

  while (flag[next].load(order_flag) && turn.load(order_flag) == next);
}

void exit_critical_section(int id, std::memory_order order) {
  flag[id].store(false, order);
}

// =============================================================================

void sum_part(int tid, std::memory_order order_flag,
              std::memory_order order_turn) {
  long start = tid * (SEQ_LENGTH / THREADS_AMMOUNT);
  long end = start + (SEQ_LENGTH / THREADS_AMMOUNT);
  long private_sum = 0;

  for (long i = start; i < end; i++) {
    private_sum += i;
  }

  enter_critical_section(tid, order_flag, order_turn);
  g_sum += private_sum;
  exit_critical_section(tid, order_flag);
}

int main() {
  for (int i = 0; i < THREADS_AMMOUNT; i++) {
    flag[i].store(false, std::memory_order_relaxed);
  }

  std::vector<std::thread> threads;

  std::vector<std::pair<std::memory_order, std::string>> orders = {
      {std::memory_order_relaxed, "relaxed"},
      {std::memory_order_acquire, "acquire"},
      {std::memory_order_release, "release"},
      {std::memory_order_seq_cst, "seq_cst"}};

  for (const auto& [order, name] : orders) {
    g_sum = 0;

    clock_t start_time = clock();
    for (int i = 0; i < THREADS_AMMOUNT; i++) {
      threads.push_back(std::thread(sum_part, i, order, order));
    }
    for (auto& th : threads) {
      th.join();
    }
    clock_t end_time = clock();

    std::cout << "--- Memory Order: " << name << " ---" << std::endl;
    std::cout << "Sum: " << g_sum << std::endl;
    std::cout << "Time: " << (double)(end_time - start_time) / CLOCKS_PER_SEC
              << " sec" << std::endl;

    threads.clear();
  }

  return 0;
}
