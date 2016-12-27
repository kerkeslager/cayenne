/* fix yield on OS X */
#if defined(__APPLE__) || defined(__MACH__)
#define pthread_yield() pthread_yield_np()
#endif

#include <stdlib.h>

#include "program.c"

int main()
{
  Program program = Program_init();
  Program_run(&program, 8);
  return EXIT_SUCCESS;
}
