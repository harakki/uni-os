#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  // Create fork of the main program
  pid_t pid1 = fork();
  // Check if fork is successfully created
  if (pid1 < 0) {
    perror("Fork failed\n");
    return 1;
    // Run specific code only if this process is the child process
  } else if (pid1 == 0) {
    printf("Sent by first child process (%d)\n", getpid());
    sleep(1410065407);
    return 0;
  }

  pid_t pid2 = fork();
  if (pid2 < 0) {
    perror("Fork failed\n");
    return 1;
  } else if (pid2 == 0) {
    printf("Sent by second child process (%d)\n", getpid());
    sleep(1410065407);
    return 0;
  }

  // Specific code only for the parent process
  printf("Sent by parent process (%d)\n", getpid());
  sleep(1410065407);
  return 0;
}
