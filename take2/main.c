/* fix yield on OS X */
#if defined(__APPLE__) || defined(__MACH__)
#define pthread_yield() pthread_yield_np()
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cayenne_thread.c"
#include "cayenne_thread_mpmc_queue.c"
#include "data_stack.c"
#include "instruction.c"
#include "mpmc_queue.c"
#include "program.c"

#define CHUNK 256

// TODO Clean up the copy/pasted code
int read_file(FILE* fp, InstructionCode** buf) 
{
  int n, np, r;
  InstructionCode* b;
	InstructionCode* b2;

  n = CHUNK;
  np = n;
  b = malloc(sizeof(InstructionCode)*n);
  while ((r = fread(b, sizeof(char), CHUNK, fp)) > 0) {
    n += r;
    if (np - n < CHUNK) { 
      np *= 2;                      // buffer is too small, the next read could overflow!
      b2 = malloc(np*sizeof(char));
      memcpy(b2, b, n * sizeof(char));
      free(b);
      b = b2;
    }
  }
  *buf = b;
  return n;
}

int main(int argc, char** argv)
{
  InstructionCode** entry_points = malloc(sizeof(InstructionCode*) * (argc - 1));

  int i;
  for(i = 1; i < argc; i++)
  {
    FILE* fp = fopen(argv[i], "rb");
    if(fp == NULL)
    {
      printf("Error opening file \"%s\"", argv[i]);
      exit(1);
    }

		read_file(fp, entry_points + i - 1);
  }

  Program program = Program_init(argc - 1, entry_points);
  Program_run(&program, 8);
  return EXIT_SUCCESS;
}
