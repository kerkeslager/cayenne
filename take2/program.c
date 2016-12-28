#ifndef PROGRAM_C
#define PROGRAM_C

#include <pthread.h>
#include <stdbool.h>

#include "program.t"

// TODO Remove these when we get real threads to execute
#include "cayenne_thread.t"
#include "data_stack.h"
#include <stdlib.h>

Program Program_init()
{
  Program program;
  program.running_pthread_count = 0;
  CayenneThreadMPMCQueue_init(&(program.cayenneThreadQueue));

  // TODO This is a dummy program; replace with one loaded from a file
  InstructionCode* startInstructionCode = malloc(sizeof(InstructionCode) * 6);
  startInstructionCode[0] = PUSH_10;
  startInstructionCode[1] = PUSH_20;
  startInstructionCode[2] = NOOP;
  startInstructionCode[3] = PRINT;
  startInstructionCode[4] = PRINT;
  startInstructionCode[5] = HALT;

  // TODO This is dummy data, remove it when we get real threads to execute
  CayenneThread* a;
  a = malloc(sizeof(CayenneThread));
  a->id = 'A';
  CayenneThread_init(a, startInstructionCode);
  DataStack_push(&(a->dataStack), 10);
  CayenneThreadMPMCQueue_enqueue((&program.cayenneThreadQueue), a);

  CayenneThread* b;
  b = malloc(sizeof(CayenneThread));
  b->id = 'B';
  CayenneThread_init(b, startInstructionCode);
  DataStack_push(&(b->dataStack), 10);
  CayenneThreadMPMCQueue_enqueue((&program.cayenneThreadQueue), b);

  CayenneThread* c;
  c = malloc(sizeof(CayenneThread));
  c->id = 'C';
  CayenneThread_init(c, startInstructionCode);
  DataStack_push(&(c->dataStack), 20);
  CayenneThreadMPMCQueue_enqueue((&program.cayenneThreadQueue), c);

  return program;
}

void _Program_incrementRunningThreadCount(Program* self)
{
  size_t old;

  do
  {
    old = self->running_pthread_count;
  }
  while(!__sync_bool_compare_and_swap(&(self->running_pthread_count), old, old + 1));
}

void _Program_decrementRunningThreadCount(Program* self)
{
  size_t old;

  do
  {
    old = self->running_pthread_count;
  }
  while(!__sync_bool_compare_and_swap(&(self->running_pthread_count), old, old - 1));
}

void* _pthread(void* arg)
{
	Program* self = (Program*)arg;
  bool running = true;
  CayenneThread* cayenne_thread = NULL;

  while(running || self->running_pthread_count > 0)
  {
    if(!running)
    {
      running = true;
      _Program_incrementRunningThreadCount(self);
    }

    cayenne_thread = CayenneThreadMPMCQueue_dequeue(&(self->cayenneThreadQueue));

    if(cayenne_thread == NULL)
    {
      running = false;
      _Program_decrementRunningThreadCount(self);
      pthread_yield();
    }

    else
    {
      CayenneThreadProgressReport progress_report = CayenneThread_progress(cayenne_thread);

      if(CayenneThreadProgressReport_isHalted(&progress_report))
      {
        CayenneThread_destroy(cayenne_thread);
      }
      else
      {
        CayenneThreadMPMCQueue_enqueue(&(self->cayenneThreadQueue), cayenne_thread);
      }
    }
  }

  return NULL;
}

void Program_run(Program* self, uint8_t pthread_count)
{
  self->running_pthread_count = pthread_count;

  pthread_t pthreads[pthread_count];
  int rc, i;

  // Spawn the threads
  for(i = 0; i < pthread_count; i++)
  {
    rc = pthread_create(&pthreads[i], NULL, _pthread, self);
  }

  // Wait for the threads to finish
  for(i = 0; i < pthread_count; i++)
  {
    rc = pthread_join(pthreads[i], NULL);
  }
}

#endif

