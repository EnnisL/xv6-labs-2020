// clang-format off
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
// clang-format on

#define STDIN_FIFENO 0
#define STDOUT_FIFENO 1
#define STDERR_FIFENO 2

#define READ 0
#define WRITE 1

int main(int argc, char *argv[]) {
  if (argc > 1) {
    fprintf(STDERR_FIFENO, "primes: no need for argument.\n");
    exit(1);
  }

  int idx = 0;
  int nums[36];
  int p[2];
  for (int i = 2; i <= 35; i++) nums[idx++] = i;

  while (idx > 0) {
    pipe(p);

    int pid = fork();
    if (pid < 0) {
      fprintf(STDERR_FIFENO, "fail to fork\n");
      exit(1);
    }

    if (pid > 0) {
      // emit nums from left in parent process.
      close(p[READ]);
      for (int i = 0; i < idx; i++) write(p[WRITE], &nums[i], sizeof(int));
      close(p[WRITE]);
      wait(0);
      exit(0);
    } else {
      // print and filter out in child process.
      close(p[WRITE]);
      int tmp, prime;
      idx = -1;
      while (read(p[READ], &tmp, sizeof(int))) {
        if (idx == -1) {
          prime = tmp;
          printf("prime %d\n", prime);
          idx++;
        } else {
          if (tmp % prime != 0) nums[idx++] = tmp;
        }
      }
      // should not exit() in this process
      close(p[READ]);
    }  // end of else
  }    // end of while

  exit(0);
  return 0;
}