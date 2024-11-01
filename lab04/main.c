#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void create_process(const char *program) {
  pid_t pid = fork();

  if (pid < 0) {
    perror("fork failed");
    exit(EXIT_FAILURE);
  }

  // Код дочернего процесса
  if (pid == 0) {
    // Замещаем процесс новой программой
    execlp(program, "/dev/null", (char *)NULL);
    perror("execlp failed");
    exit(EXIT_FAILURE);
  } else {
    // Код родительского процесса
    printf("Parent PID: %d, Child PID: %d\n", getpid(), pid);
  }
}

int main() {
  // Запускаем несколько программ
  create_process("echo");
  create_process("tail");
  create_process("cat");

  // Ожидание завершения дочерних процессов
  while (wait(NULL) > 0);

  return 0;
}
