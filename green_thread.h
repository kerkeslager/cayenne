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
  MPSCQueue* messageQueue;
  DataStack dataStack;
  ReturnStack returnStack;
};

void GreenThread_executeInstruction(GreenThread* self)
{
  Opcode opcode = self->currentInstruction->opcode;

  switch(opcode)
  {
    case OPCODE_NOOP:
      break;

    case OPCODE_DROP:
      DataStack_pop(&(self->dataStack));
      break;

    default:
      fprintf(stderr, "Invalid opcode %i.\n", opcode);
      exit(1);
  }
}

#endif
