#ifndef CAYENNE_THREAD_C
#define CAYENNE_THREAD_C

#include "data_stack.h"
#include "cayenne_thread.t"

#include <stdio.h>

void CayenneThread_init(CayenneThread* self)
{
  DataStack_init(&(self->dataStack));
}

void CayenneThread_destroy(CayenneThread* self)
{
  DataStack_destroy(&(self->dataStack));
}

CayenneThreadProgressReport CayenneThread_progress(CayenneThread* self)
{
  // TODO This is a dummy test instruction
  int state = DataStack_pop(&(self->dataStack));
  printf("Thread: %c State: %i\n", self->id, state);
  DataStack_push(&(self->dataStack), state - 1);

  CayenneThreadProgressReport result;
  result.halted = state == 0;
  return result;
}

bool CayenneThreadProgressReport_isHalted(CayenneThreadProgressReport* self)
{
  return self->halted;
}

#endif
