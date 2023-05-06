// Create a zombie process that
// must be reparented at exit.

// clang-format off
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
// clang-format on

int main(void) {
  if (fork() > 0) sleep(5);  // Let child exit before parent.
  exit(0);
}
