// clang-format off
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
// clang-format on

#define READ 0
#define WRITE 1

#define STDIN_FIFENO 0
#define STDOUT_FIFENO 1
#define STDERR_FIFENO 2

int main(int argc, char const *argv[]) {
  if (argc > 1) {
    fprintf(STDERR_FIFENO, "pingpong: no need for argument.\n");
    exit(1);
  }

  char buf[512];
  int p2c[2], c2p[2];

  pipe(p2c);
  pipe(c2p);

  int pid = fork();
  if (pid < 0) {
    fprintf(STDERR_FIFENO, "fail to fork\n");
    exit(1);
  }

  /**
   * 1. The parent should seed a byte to the child -> the child should print
   * "<pid>: received ping", where <pid> is its process id, write the byte on
   * the pipe to the parent, and exit.
   *
   * 2. The parent should read the byte from the child, print "<pid>: received
   * pong", and exit.
   *
   */

  if (pid == 0) {
    close(p2c[READ]);
    fprintf(STDOUT_FIFENO, "%d: received ping\n", getpid());
    write(c2p[WRITE], buf, 1);
    close(c2p[WRITE]);
    exit(0);
  } else {
    close(p2c[WRITE]);
    read(c2p[READ], buf, 1);
    close(c2p[READ]);
    fprintf(STDOUT_FIFENO, "%d: received pong\n", getpid());
    exit(0);
  }

  // should not reach here
  exit(1);
}