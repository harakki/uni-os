#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>

atomic_int sh = 0;

void* my_thread0() {
  int i = 0;
  for (; i < 100; i++) {
    atomic_fetch_add(&sh, 1);
  }
}

void* my_thread1() {
  int i = 0;
  for (; i < 100; i++) {
    atomic_fetch_add(&sh, 2);
  }
}

int main() {
  pthread_t th_id[2];
  pthread_create(&th_id[0], NULL, &my_thread0, NULL);
  pthread_create(&th_id[1], NULL, &my_thread1, NULL);
  pthread_join(th_id[0], NULL);
  pthread_join(th_id[1], NULL);
  printf("%i\n", atomic_load(&sh));
  return 0;
}
