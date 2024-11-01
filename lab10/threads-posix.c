#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

// Каждый поток выполняет 200.000 сумированний
#define THREADS_AMMOUNT 12
#define SEQ_LENGTH 2400000

long g_sum = 0;
bool flag[THREADS_AMMOUNT] = {false};
int turn;

// ============================ Алгоритм Петерсона ============================

void enter_critical_section(int id) {
  int next = (id + 1) % THREADS_AMMOUNT;
  flag[id] = true;
  turn = next;
  while (flag[next] && turn == next);
}

void exit_critical_section(int id) { flag[id] = false; }

// ============================================================================

void* sum_part(void* arg) {
  int id = (int)(long)arg;

  long start = ((long)arg) * (SEQ_LENGTH / THREADS_AMMOUNT);
  long end = start + (SEQ_LENGTH / THREADS_AMMOUNT);
  long private_sum = 0;

  for (long i = start; i < end; i++) {
    private_sum += i;
  }

  enter_critical_section(id);
  g_sum += private_sum;
  exit_critical_section(id);

  return NULL;
}

int main(void) {
  pthread_t threads[THREADS_AMMOUNT];

  clock_t start_time = clock();
  for (long i = 0; i < THREADS_AMMOUNT; i++) {
    pthread_create(&threads[i], NULL, sum_part, (void*)i);
  }
  for (int i = 0; i < THREADS_AMMOUNT; i++) {
    pthread_join(threads[i], NULL);
  }
  clock_t end_time = clock();

  printf("--- POSIX ---\n%ld\n", g_sum);
  printf("%f sec\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

  return 0;
}
