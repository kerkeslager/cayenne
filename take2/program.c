#ifndef PROGRAM_C
#define PROGRAM_C

#include <pthread.h>
#include <stdbool.h>

#include "program.t"

Program Program_init()
{
  Program program;
  program.running_pthread_count = 0;
  return program;
}

void Program_incrementRunningThreadCount(Program* self)
{
  size_t old;

  do
  {
    old = self->running_pthread_count;
  }
  while(!__sync_bool_compare_and_swap(&(self->running_pthread_count), old, old + 1));
}

void Program_decrementRunningThreadCount(Program* self)
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
  void* cayenne_thread = NULL;

  while(running || self->running_pthread_count > 0)
  {
    if(!running)
    {
      running = true;
      Program_incrementRunningThreadCount(self);
    }

    // TODO Not yet implemented
    // cayenne_thread = CayenneThreadMPMCQueue_pop(self->cayenneThreadQueue);

    if(cayenne_thread == NULL)
    {
      running = false;
      Program_decrementRunningThreadCount(self);
      pthread_yield();
    }

    else
    {
      // TODO Not yet implemented
      // CayenneThread_progress(cayenne_thread);
      // CayenneThreadMPMCQueue_push(self->cayenneThreadQueue, cayenne_thread);
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

