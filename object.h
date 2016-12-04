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

#ifndef OBJECT_H
#define OBJECT_H

#include "instruction.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum Type
{
  TYPE_BOOLEAN,
  TYPE_CLOSURE,
  TYPE_INTEGER
};
typedef enum Type Type;

union Instance;
typedef union Instance Instance;
union Instance
{
  bool boolean;
  Instruction* closure;
  int32_t integer;
};

struct Object;
typedef struct Object Object;
struct Object
{
  size_t referenceCount;
  Type type;
  Instance instance;
};

void Object_initialize(Object* self, Type type, Instance instance)
{
  self->referenceCount = 1;
  self->type = type;
  self->instance = instance;
}

Object* Object_create(Type type, Instance instance)
{
  Object* result = malloc(sizeof(Object));
  Object_initialize(result, type, instance);
  return result;
}

Object* Object_reference(Object* o)
{
  (o->referenceCount)++;
  return o;
}

void Object_dereference(Object* o)
{
  (o->referenceCount)--;

  if(o->referenceCount == 0)
  {
    free(o);
  }
}

#endif
