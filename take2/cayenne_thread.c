#ifndef CAYENNE_THREAD_C
#define CAYENNE_THREAD_C

#include "data_stack.h"
#include "instruction.h"

#include "cayenne_thread.t"
#include "instruction.t"

#include <stdio.h>

void CayenneThread_init(CayenneThread* self, InstructionCode* start)
{
  DataStack_init(&(self->dataStack));
  self->instruction = start;
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
