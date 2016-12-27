#ifndef INSTRUCTION_C
#define INSTRUCTION_C

#include "instruction.t"

CayenneThreadProgressReport Instruction_decrementPrintAndHaltIfZero(CayenneThread* self)
{
  // TODO This is a dummy test instruction
  int state = DataStack_pop(&(self->dataStack));
  printf("Thread: %c State: %i\n", self->id, state);
  DataStack_push(&(self->dataStack), state - 1);

  CayenneThreadProgressReport result;
  result.halted = state == 0;
  return result;
}

CayenneThreadProgressReport Instruction_noop(CayenneThread* self)
{
  CayenneThreadProgressReport result;
  result.halted = false;
  return result;
}

CayenneThreadProgressReport Instruction_halt(CayenneThread* self)
{
  CayenneThreadProgressReport result;
  result.halted = true;
  return result;
}

CayenneThreadProgressReport Instruction_push10(CayenneThread* self)
{
  // TODO Remove this instruction
  DataStack_push(&(self->dataStack), 10);
  return Instruction_noop(self);
}

CayenneThreadProgressReport Instruction_push20(CayenneThread* self)
{
  // TODO Remove this instruction
  DataStack_push(&(self->dataStack), 20);
  return Instruction_noop(self);
}

CayenneThreadProgressReport Instruction_print(CayenneThread* self)
{
  int output = DataStack_pop(&(self->dataStack));
  printf("Thread:%c Output:%i\n", self->id, output);
  return Instruction_noop(self);
}

Instruction InstructionCode_toInstruction(InstructionCode* self)
{
  switch(*self)
  {
    case NOOP:
      return &Instruction_noop;

    case HALT:
      return &Instruction_halt;

    case PUSH_10:
      return &Instruction_push10;

    case PUSH_20:
      return &Instruction_push20;

    case PRINT:
      return &Instruction_print;

    default:
      printf("An error occurred\n");
      exit(0);
  }
}

#endif
