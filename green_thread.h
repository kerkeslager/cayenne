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

#ifndef GREEN_THREAD_H
#define GREEN_THREAD_H

#include "environment.h"
#include "instruction.h"
#include "mpsc_queue.h"
#include "stack.h"

#include <stdlib.h>

struct GreenThread;
typedef struct GreenThread GreenThread;
struct GreenThread
{
  bool running;
  Instruction* currentInstruction;
  MPSCQueue messageQueue;
  DataStack dataStack;
  ReturnStack returnStack;
  Environment environment;
};

void GreenThread_initialize(GreenThread* self)
{
  self->running = false;
  MPSCQueue_initialize(&(self->messageQueue));
  DataStack_initialize(&(self->dataStack));
  ReturnStack_initialize(&(self->returnStack));
  Environment_initialize(&(self->environment));
}

void GreenThread_executeSwap(GreenThread* self)
{
  DataStack* dataStack = &(self->dataStack);
  Object* a = DataStack_pop(dataStack);
  Object* b = DataStack_pop(dataStack);
  DataStack_push(dataStack, a);
  DataStack_push(dataStack, b);
}

void GreenThread_executeLoad(GreenThread* self, Symbol* symbol)
{
  DataStack* dataStack = &(self->dataStack);
  Environment* environment = &(self->environment);
  DataStack_push(dataStack, Environment_get(environment, symbol));
}

void GreenThread_executeStore(GreenThread* self, Symbol* symbol)
{
  DataStack* dataStack = &(self->dataStack);
  Environment* environment = &(self->environment);
  Environment_set(environment, symbol, DataStack_pop(dataStack));
}

void GreenThread_executeInstruction(GreenThread* self, Instruction* instruction)
{
  Opcode opcode = instruction->opcode;

  switch(opcode)
  {
    case OPCODE_NOOP:
      break;

    case OPCODE_DROP:
      DataStack_pop(&(self->dataStack));
      break;

    case OPCODE_SWAP:
      GreenThread_executeSwap(self);
      break;

    case OPCODE_LOAD:
      GreenThread_executeLoad(self, instruction->symbol);
      break;

    case OPCODE_STORE:
      GreenThread_executeStore(self, instruction->symbol);
      break;

    default:
      fprintf(stderr, "Invalid opcode %i.\n", opcode);
      exit(1);
  }
}

void GreenThread_executeCurrentInstruction(GreenThread* self)
{
  GreenThread_executeInstruction(self, self->currentInstruction);
  self->currentInstruction++;
}

#endif
