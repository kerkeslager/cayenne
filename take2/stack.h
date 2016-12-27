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

#include <stdlib.h>

#define StackH(stackType, itemType) \
  void stackType##_init(stackType*); \
  void stackType##_destroy(stackType*); \
  void stackType##_push(stackType*, itemType); \
  itemType stackType##_pop(stackType*);

#define StackT(stackType, itemType) \
  struct _##stackType##Frame; \
  typedef struct _##stackType##Frame _##stackType##Frame; \
  struct _##stackType##Frame \
  { \
    itemType item; \
    _##stackType##Frame* next; \
  }; \
   \
  struct stackType \
  { \
    _##stackType##Frame* top; \
  };

#define StackC(stackType, itemType) \
  void stackType##_init(stackType* s) \
  { \
    s->top = NULL; \
  } \
   \
  void stackType##_push(stackType* self, itemType item) \
  { \
    _##stackType##Frame* top = malloc(sizeof(_##stackType##Frame)); \
    top->item = item; \
    top->next = self->top; \
    self->top = top; \
  } \
   \
  itemType stackType##_pop(stackType* self) \
  { \
    itemType result = self->top->item; \
    _##stackType##Frame* top = self->top->next; \
    free(self->top); \
    self->top = top; \
    return result; \
  } \
   \
  void stackType##_destroy(stackType* s) \
  { \
    while(s->top != NULL) stackType##_pop(s); \
  }

#endif
