#ifndef CAYENNE_THREAD_C
#define CAYENNE_THREAD_C

#include "cayenne_thread.t"

#include <stdio.h>

CayenneThreadProgressReport CayenneThread_progress(CayenneThread* self)
{
  self->state--;
  printf("Thread: %c State: %i\n", self->id, self->state);

  CayenneThreadProgressReport result;
  result.halted = self->state == 0;
  return result;
}

bool CayenneThreadProgressReport_isHalted(CayenneThreadProgressReport* self)
{
  return self->halted;
}

#endif
