/* fix yield on OS X */
#if defined(__APPLE__) || defined(__MACH__)
#define pthread_yield() pthread_yield_np()
#endif

#include <stdlib.h>

#include "cayenne_thread.c"
#include "cayenne_thread_mpmc_queue.c"
#include "data_stack.c"
#include "instruction.c"
#include "mpmc_queue.c"
#include "program.c"

int main()
{
  // TODO This is a dummy program; replace with one loaded from a file
  InstructionCode* start_instruction_code = malloc(sizeof(InstructionCode) * 6);
  start_instruction_code[0] = PUSH_10;
  start_instruction_code[1] = PUSH_20;
  start_instruction_code[2] = NOOP;
  start_instruction_code[3] = PRINT;
  start_instruction_code[4] = PRINT;
  start_instruction_code[5] = HALT;

  // TODO Better name for this
  int8_t start_module_count = 3;
  InstructionCode** start_modules = malloc(sizeof(InstructionCode*) * start_module_count);
  start_modules[0] = start_instruction_code;
  start_modules[1] = start_instruction_code;
  start_modules[2] = start_instruction_code;

  Program program = Program_init(start_module_count, start_modules);
  Program_run(&program, 8);
  return EXIT_SUCCESS;
}
