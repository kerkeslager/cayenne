/* This file is part of Cayenne.

Cayenne is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Cayenne is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Cayenne.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef PROGRAM_H
#define PROGRAM_H

#include "green_thread.h"
#include "mpmc_queue.h"

#include <pthread.h>

struct Module;
typedef struct Module Module;
struct Module
{
  size_t count;
  Instruction* instructions;
};

struct ModuleDictionaryNode;
typedef struct ModuleDictionaryNode ModuleDictionaryNode;
struct ModuleDictionaryNode
{
  Symbol* key;
  Module* value;
  ModuleDictionaryNode* next;
};

struct ModuleDictionary;
typedef struct ModuleDictionary ModuleDictionary;
struct ModuleDictionary
{
  ModuleDictionaryNode* first;
};

struct Program;
typedef struct Program Program;
struct Program
{
  size_t threadCount;
  MPMCQueue threadQueue;
  ModuleDictionary modules;
};

#define PTHREAD_COUNT 1

void* driverLoop(void* arg)
{
  Program* program = (Program*) arg;

  MPMCQueue* threadQueue = &(program->threadQueue);

  /* TODO Figure out how to join this pthread. */
  while(true)
  {
    /* TODO This will block until a GreenThread is on the queue, which may be
    never. Figure out how to check threadCount before yielding in the
    dequeue. */
    GreenThread* thread = MPMCQueue_dequeue(threadQueue);
    GreenThread_executeCurrentInstruction(thread);
    MPMCQueue_enqueue(threadQueue, thread);
  }

  return NULL;
}

void Program_run(Program* self)
{
  pthread_t pthreads[PTHREAD_COUNT];
  int rc, i;

  // Spawn the threads
  for(i = 0; i < PTHREAD_COUNT; i++)
  {
    rc = pthread_create(&pthreads[i], NULL, driverLoop, self);
  }

  // Wait for the threads to finish
  for(i = 0; i < PTHREAD_COUNT; i++)
  {
    rc = pthread_join(pthreads[i], NULL);
  }
}

void Program_initialize(Program* self, GreenThread* start)
{
  self->threadCount = 0; // These bedsheets suck
  MPMCQueue_initialize(&(self->threadQueue));
  MPMCQueue_enqueue(&(self->threadQueue), start);
}

#endif
