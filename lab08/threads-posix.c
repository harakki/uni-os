#include <pthread.h>
#include <stdio.h>
#include <time.h>

// Каждый поток выполняет 200.000 сумированний
#define THREADS_AMMOUNT 12
#define SEQ_LENGTH 2400000

long g_sum = 0;
pthread_mutex_t g_mtx;

void* sum_part(void* arg) {
  // Вычисление диапазона чисел для потока
  long start = ((long)arg) * (SEQ_LENGTH / THREADS_AMMOUNT);
  long end = start + (SEQ_LENGTH / THREADS_AMMOUNT);
  long private_sum = 0;

  for (long i = start; i < end; i++) {
    private_sum += i;
  }

  // Для избежания одновременного доступа к переменной потоками, используем
  // мьютекс (ошибка race conditions)
  pthread_mutex_lock(&g_mtx);
  g_sum += private_sum;
  pthread_mutex_unlock(&g_mtx);

  return NULL;
}

int main(void) {
  // Инициализация потоков
  pthread_t threads[THREADS_AMMOUNT];
  pthread_mutex_init(&g_mtx, NULL);

  // Многопоточное вычисление
  clock_t start_time = clock();
  for (long i = 0; i < THREADS_AMMOUNT; i++) {
    pthread_create(&threads[i], NULL, sum_part, (void*)i);
  }
  for (int i = 0; i < THREADS_AMMOUNT; i++) {
    pthread_join(threads[i], NULL);
  }
  clock_t end_time = clock();

  // Результат
  printf("--- POSIX ---\n%ld\n", g_sum);
  printf("%f sec\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

  pthread_mutex_destroy(&g_mtx);
  return 0;
}
