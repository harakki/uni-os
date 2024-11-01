#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int sh = 0;
int turn = 0;
// int flag[2]={1,1};

void* my_thread0() {
  int i = 0;
  for (; i < 100; i++) {
    // BUG: Есть риск возникновения race condition в случае взаимной блокировки.
    // Стоит использовать мьютекс
    while (turn);  // spinlock
    sh++;          // критическая область
    turn = 1;
    // usleep(1);  // некритическая область
  }
  // flag[0]=0;
}

void* my_thread1() {
  int i = 0;
  for (; i < 100; i++) {
    while (!turn);
    sh += 2;
    turn = 0;
    // usleep(100);
  }
  // flag[1]=0;
}

int main() {
  pthread_t th_id[2];
  pthread_create(&th_id[0], NULL, &my_thread0, NULL);
  pthread_create(&th_id[1], NULL, &my_thread1, NULL);
  // while(flag[0] || flag[1]);
  pthread_join(th_id[0], NULL);
  pthread_join(th_id[1], NULL);
  printf("%i\n", sh);
  return 0;
}
