#ifndef PROGRAM_C
#define PROGRAM_C

#include <pthread.h>
#include <stdbool.h>

#include "program.t"

// TODO Remove these when we get real threads to execute
#include "cayenne_thread.t"
#include "data_stack.h"
#include <stdlib.h>

Program Program_init(int8_t module_count, InstructionCode** module_instructions)
{
  Program program;
  program.running_pthread_count = 0;
  CayenneThreadMPMCQueue_init(&(program.cayenneThreadQueue));

  int8_t i;

  for(i = 0; i < module_count; i++)
  {
    CayenneThread* module_thread = malloc(sizeof(CayenneThread));
    module_thread->id = i + 65;
    CayenneThread_init(module_thread, module_instructions[i]);
    CayenneThreadMPMCQueue_enqueue((&program.cayenneThreadQueue), module_thread);
  }

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

