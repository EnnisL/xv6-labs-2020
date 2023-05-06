// clang-format off
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
// clang-format on

#define STDIN_FIFENO 0

int main(int argc, char* argv[]) {
  char buf[10];
  char* cur[MAXARG];
  int i = 0, j = 0;
  for (i = 0; i < argc - 1; i++) {
    cur[i] = argv[i + 1];
  }
  cur[i] = malloc(MAXARG);

  while (read(STDIN_FIFENO, buf, 1) == 1) {
    if (i == argc - 1) printf("%s,%s\n", buf, cur[i]);

    switch (buf[0]) {
      case '\n': {
        // To read individual lines of input, read a character at a time until a
        // newline ('\n') appears.
        if (fork() == 0) {
          // Use fork and exec to invoke the command on each line of input.
          exec(cur[0], cur);
          exit(0);
        }
        // Use wait in the parent to wait for the child to complete the command.
        wait(0);

        // free() does not change the value of the memory, it just makes it
        // available for allocation by other programs.
        // Manually set the memory to 0 using the memset function
        for (int k = argc - 1; k <= i; k++) {
          memset(cur[k], 0, MAXARG);
          free(cur[k]);
        }

        i = argc - 1;
        cur[i] = malloc(MAXARG);
        j = 0;
        break;
      }
      case ' ': {
        i++;
        j = 0;
        if (i < MAXARG) cur[i] = malloc(MAXARG);
        break;
      }
      default: {
        cur[i][j++] = buf[0];
        break;
      }
    }
  }
  exit(0);
}