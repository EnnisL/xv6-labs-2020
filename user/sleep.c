// clang-format off
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
// clang-format on

#define STDERR_FIFENO 2

int parse_argv(const char *argv)
{
  const char *p = argv;
  while (*p)
  {
    if (*p < '0' || *p > '9')
    {
      return -1;
    }
    p++;
  }
  return atoi(argv);
}

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    fprintf(STDERR_FIFENO, "sleep: lack of argument.\n");
    exit(1);
  }

  int n = parse_argv(argv[1]);
  if (n < 0)
  {
    fprintf(STDERR_FIFENO, "sleep: sleep time should be greater than 0.\n");
    exit(1);
  }

  sleep(n);
  exit(0);
}