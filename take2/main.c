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
int read_file(FILE* fp, InstructionCode** output_buffer)
{
  InstructionCode read_buffer[CHUNK];

  size_t allocated = CHUNK;
  InstructionCode* result = calloc(sizeof(InstructionCode), CHUNK);

  size_t bytes_read_in_chunk;
  size_t total_bytes_read = 0;

  while((bytes_read_in_chunk = fread(read_buffer, sizeof(InstructionCode), CHUNK, fp)) > 0)
  {
    if(total_bytes_read + bytes_read_in_chunk > allocated)
    {
      allocated *= 2;
      result = realloc(result, allocated);
    }

    memcpy(result + total_bytes_read, read_buffer, bytes_read_in_chunk);

    total_bytes_read += bytes_read_in_chunk;
  }

  *output_buffer = result;
  return total_bytes_read;
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
