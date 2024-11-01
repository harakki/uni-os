#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 5

typedef enum { FINITE, CYCLIC } BufferType;

typedef struct {
  int queue[BUFFER_SIZE];
  int front;
  int rear;
  int count;
  sem_t empty;
  sem_t full;
} finite_buffer_t;

typedef struct {
  int buffer[BUFFER_SIZE];
  int head;
  int tail;
  int count;
  sem_t empty;
  sem_t full;
} cyclic_buffer_t;

finite_buffer_t g_finite_buffer;
cyclic_buffer_t g_cyclic_buffer;

void init_finite_buffer(finite_buffer_t* buf) {
  buf->front = 0;
  buf->rear = 0;
  buf->count = 0;
  sem_init(&buf->empty, 0, BUFFER_SIZE);
  sem_init(&buf->full, 0, 0);
}

void init_cyclic_buffer(cyclic_buffer_t* buf) {
  buf->head = 0;
  buf->tail = 0;
  buf->count = 0;
  sem_init(&buf->empty, 0, BUFFER_SIZE);
  sem_init(&buf->full, 0, 0);
}

void add_finite(finite_buffer_t* buf, int item) {
  sem_wait(&buf->empty);
  buf->queue[buf->rear] = item;
  buf->rear = (buf->rear + 1) % BUFFER_SIZE;
  buf->count++;
  sem_post(&buf->full);
}

void add_cyclic(cyclic_buffer_t* buf, int item) {
  sem_wait(&buf->empty);
  buf->buffer[buf->head] = item;
  buf->head = (buf->head + 1) % BUFFER_SIZE;
  buf->count++;
  sem_post(&buf->full);
}

int remove_finite(finite_buffer_t* buf) {
  sem_wait(&buf->full);
  int item = buf->queue[buf->front];
  buf->front = (buf->front + 1) % BUFFER_SIZE;
  buf->count--;
  sem_post(&buf->empty);
  return item;
}

int remove_cyclic(cyclic_buffer_t* buf) {
  sem_wait(&buf->full);
  int item = buf->buffer[buf->tail];
  buf->tail = (buf->tail + 1) % BUFFER_SIZE;
  buf->count--;
  sem_post(&buf->empty);
  return item;
}

void* producer(void* arg) {
  BufferType type = *(BufferType*)arg;
  for (int i = 0; i < 10; i++) {
    if (type == FINITE) {
      printf("\e[31mFINITE [PRODUCER]: %d\n", i);
      add_finite(&g_finite_buffer, i);
    } else {
      printf("\e[32mCYCLIC [PRODUCER]: %d\n", i);
      add_cyclic(&g_cyclic_buffer, i);
    }
  }
  return NULL;
}

// Потребитель
void* consumer(void* arg) {
  BufferType type = *(BufferType*)arg;
  for (int i = 0; i < 10; i++) {
    int item;
    if (type == FINITE) {
      item = remove_finite(&g_finite_buffer);
      printf("\e[31mFINITE [\e[0mCONSUMER\e[31m]: %d\n", item);
    } else {
      item = remove_cyclic(&g_cyclic_buffer);
      printf("\e[32mCYCLIC [\e[0mCONSUMER\e[32m]: %d\n", item);
    }
  }
  return NULL;
}

int main() {
  init_finite_buffer(&g_finite_buffer);
  init_cyclic_buffer(&g_cyclic_buffer);

  BufferType finite_type = FINITE;
  BufferType cyclic_type = CYCLIC;

  pthread_t producer1, consumer1;
  pthread_create(&producer1, NULL, producer, (void*)&finite_type);
  pthread_create(&consumer1, NULL, consumer, (void*)&finite_type);

  pthread_t producer2, consumer2;
  pthread_create(&producer2, NULL, producer, (void*)&cyclic_type);
  pthread_create(&consumer2, NULL, consumer, (void*)&cyclic_type);

  pthread_join(producer1, NULL);
  pthread_join(consumer1, NULL);
  pthread_join(producer2, NULL);
  pthread_join(consumer2, NULL);

  return 0;
}
