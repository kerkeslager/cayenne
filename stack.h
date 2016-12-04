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

#ifndef STACK_H
#define STACK_H

#include "instruction.h"
#include "object.h"

#include <stdlib.h>

#define Stack(stackType, itemType)                            \
  struct stackType##Frame;                                    \
  typedef struct stackType##Frame stackType##Frame;           \
  struct stackType##Frame                                     \
  {                                                           \
    itemType item;                                            \
    stackType##Frame* next;                                   \
  };                                                          \
                                                              \
  struct stackType                                            \
  {                                                           \
    stackType##Frame* top;                                    \
  };                                                          \
                                                              \
  void stackType##_initialize(stackType* s)                   \
  {                                                           \
    s->top = NULL;                                            \
  }                                                           \
                                                              \
  void stackType##_push(stackType* self, itemType item)       \
  {                                                           \
    stackType##Frame* top = malloc(sizeof(stackType##Frame)); \
    top->item = item;                                         \
    top->next = self->top;                                    \
    self->top = top;                                          \
  }                                                           \
                                                              \
  itemType stackType##_pop(stackType* self)                   \
  {                                                           \
    itemType result = self->top->item;                        \
    stackType##Frame* top = self->top->next;                  \
    free(self->top);                                          \
    self->top = top;                                          \
    return result;                                            \
  }                                                           \
                                                              \
  void stackType##_destroy(stackType* s)                      \
  {                                                           \
    while(s->top != NULL) stackType##_pop(s);                 \
  }

struct DataStack;
typedef struct DataStack DataStack;
struct ReturnStack;
typedef struct ReturnStack ReturnStack;

Stack(DataStack, Object*)
Stack(ReturnStack, Instruction*)

#undef Stack

#endif
