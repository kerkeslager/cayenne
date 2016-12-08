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
  Instruction* currentInstruction;
  MPSCQueue messageQueue;
  DataStack dataStack;
  ReturnStack returnStack;
  Environment environment;
};

void GreenThread_initialize(GreenThread* self, Instruction* start)
{
  self->currentInstruction = start;
  MPSCQueue_initialize(&(self->messageQueue));
  DataStack_initialize(&(self->dataStack));
  ReturnStack_initialize(&(self->returnStack));
  Environment_initialize(&(self->environment));
}

void GreenThread_destroy(GreenThread* self)
{
  MPSCQueue_destroy(&(self->messageQueue));
  DataStack_destroy(&(self->dataStack));
  ReturnStack_destroy(&(self->returnStack));
  Environment_destroy(&(self->environment));
}

GreenThread* GreenThread_executeFork(GreenThread* self, Symbol* symbol)
{
  GreenThread* forked = malloc(sizeof(GreenThread));

  // TODO Implement a full closure
  Object* instructionObject = Environment_get(&(self->environment), symbol);
  Instruction* instruction = Object_asClosure(instructionObject);

  GreenThread_initialize(forked, instruction);

  return forked;
}

void GreenThread_executeCall(GreenThread* self, Symbol* symbol)
{
  ReturnStack* returnStack = &(self->returnStack);
  ReturnStack_push(returnStack, self->currentInstruction);

  Environment* environment = &(self->environment);

  // TODO Implement a full closure
  Object* instructionObject = Environment_get(environment, symbol);
  Instruction* instruction = Object_asClosure(instructionObject);

  self->currentInstruction = instruction;
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

void GreenThread_executeReturn(GreenThread* self)
{
  ReturnStack* returnStack = &(self->returnStack);
  self->currentInstruction = ReturnStack_pop(returnStack);
}

bool GreenThread_executeReceive(GreenThread* self)
{
  MPSCQueue* messageQueue = &(self->messageQueue);

  if(MPSCQueue_isEmpty(messageQueue))
  {
    // TODO Check the reference count of the thread and collect it if it's 0
    // If there are no references to the thread and the message queue is empty it will never progress
    return false;
  }

  DataStack* dataStack = &(self->dataStack);

  DataStack_push(dataStack, MPSCQueue_dequeue(messageQueue));

  return true;
}

struct InstructionResult;
typedef struct InstructionResult InstructionResult;
struct InstructionResult
{
  bool halt;
  bool increment;
  GreenThread* addGreenThread;
};

InstructionResult GreenThread_executeInstruction(
    GreenThread* self,
    Instruction* instruction)
{
  Opcode opcode = instruction->opcode;
  InstructionResult result;
  result.halt = false;
  result.increment = true;
  result.addGreenThread = NULL;

  switch(opcode)
  {
    case OPCODE_NOOP:
      return result;

    case OPCODE_HALT:
      result.halt = true;
      return result;

    case OPCODE_FORK:
      result.addGreenThread = GreenThread_executeFork(self, instruction->symbol);
      return result;

    case OPCODE_DROP:
      DataStack_pop(&(self->dataStack));
      return result;

    case OPCODE_SWAP:
      GreenThread_executeSwap(self);
      return result;

    case OPCODE_LOAD:
      GreenThread_executeLoad(self, instruction->symbol);
      return result;

    case OPCODE_STORE:
      GreenThread_executeStore(self, instruction->symbol);
      return result;

    case OPCODE_CALL:
      GreenThread_executeCall(self, instruction->symbol);
      return result;

    case OPCODE_RETURN:
      GreenThread_executeReturn(self);
      return result;

    case OPCODE_RECEIVE:
      result.increment = GreenThread_executeReceive(self);
      return result;

    case OPCODE_HELLO:
      printf("Hello, world\n");
      return result;

    default:
      fprintf(stderr, "Invalid opcode %i.\n", opcode);
      exit(1);
  }
}

InstructionResult GreenThread_executeCurrentInstruction(GreenThread* self)
{
  InstructionResult result = GreenThread_executeInstruction(self, self->currentInstruction);

  if(result.increment)
  {
    self->currentInstruction++;
  }

  return result;
}

#endif
