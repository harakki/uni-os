#include <ctime>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// Каждый поток выполняет 200.000 сумированний
#define THREADS_AMMOUNT 12
#define SEQ_LENGTH 2400000

long g_sum = 0;
std::mutex g_mtx;

void sum_part(int tid) {
  // Вычисление диапазона чисел для потока
  long start = tid * (SEQ_LENGTH / THREADS_AMMOUNT);
  long end = start + (SEQ_LENGTH / THREADS_AMMOUNT);
  long private_sum = 0;

  for (long i = start; i < end; i++) {
    private_sum += i;
  }

  // Для избежания одновременного доступа к переменной потоками, используем
  // мьютекс (ошибка race conditions); lock_guard() автоматически разблокирует
  // мьютекс при выходе из области видимости
  std::lock_guard<std::mutex> lock(g_mtx);
  g_sum += private_sum;
}

int main(void) {
  // Инициализация потоков
  std::vector<std::thread> threads;

  // Многопоточное вычисление
  clock_t start_time = clock();
  for (int i = 0; i < THREADS_AMMOUNT; i++) {
    threads.push_back(std::thread(sum_part, i));
  }
  for (auto& th : threads) {
    th.join();
  }
  clock_t end_time = clock();

  // Результат
  std::cout << "---  STD  ---" << std::endl << g_sum << std::endl;
  std::cout << (double)(end_time - start_time) / CLOCKS_PER_SEC << " sec"
            << std::endl;
}
