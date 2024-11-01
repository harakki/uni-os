#include <array>
#include <atomic>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>

// Каждый поток выполняет 200.000 сумированний
#define THREADS_AMMOUNT 12
#define SEQ_LENGTH 2400000

long g_sum = 0;
std::array<std::atomic<bool>, THREADS_AMMOUNT> flag;
std::atomic<int> turn;

// ============================ Алгоритм Петерсона ============================

void enter_critical_section(int id) {
  int next = (id + 1) % THREADS_AMMOUNT;
  flag[id] = true;
  turn = next;
  while (flag[next] && turn == next);
}

void exit_critical_section(int id) { flag[id] = false; }

// Функция для выхода из критической секции

void sum_part(int tid) {
  long start = tid * (SEQ_LENGTH / THREADS_AMMOUNT);
  long end = start + (SEQ_LENGTH / THREADS_AMMOUNT);
  long private_sum = 0;

  for (long i = start; i < end; i++) {
    private_sum += i;
  }

  enter_critical_section(tid);
  g_sum += private_sum;
  exit_critical_section(tid);
}

int main(void) {
  for (int i = 0; i < THREADS_AMMOUNT; i++) {
    flag[i] = false;
  }

  std::vector<std::thread> threads;

  clock_t start_time = clock();
  for (int i = 0; i < THREADS_AMMOUNT; i++) {
    threads.push_back(std::thread(sum_part, i));
  }
  for (auto& th : threads) {
    th.join();
  }
  clock_t end_time = clock();

  std::cout << "---  STD  ---" << std::endl << g_sum << std::endl;
  std::cout << (double)(end_time - start_time) / CLOCKS_PER_SEC << " sec"
            << std::endl;
}
