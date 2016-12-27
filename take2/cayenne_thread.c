#ifndef CAYENNE_THREAD_C
#define CAYENNE_THREAD_C

#include "data_stack.h"
#include "instruction.h"
#include "cayenne_thread.t"

#include <stdio.h>

void CayenneThread_init(CayenneThread* self)
{
  DataStack_init(&(self->dataStack));

  // TODO This is a dummy program; replace with one loaded from a file
  self->instruction = malloc(sizeof(InstructionCode) * 6);
  self->instruction[0] = PUSH_10;
  self->instruction[1] = PUSH_20;
  self->instruction[2] = NOOP;
  self->instruction[3] = PRINT;
  self->instruction[4] = PRINT;
  self->instruction[5] = HALT;
}

void CayenneThread_destroy(CayenneThread* self)
{
  DataStack_destroy(&(self->dataStack));
}

CayenneThreadProgressReport CayenneThread_progress(CayenneThread* self)
{
  CayenneThreadProgressReport result = InstructionCode_toInstruction(self->instruction)(self);
  self->instruction++;
  return result;
}

bool CayenneThreadProgressReport_isHalted(CayenneThreadProgressReport* self)
{
  return self->halted;
}

#endif
