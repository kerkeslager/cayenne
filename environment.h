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

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "object.h"
#include "symbol.h"

#include <stdlib.h>

struct EnvironmentFrame;
typedef struct EnvironmentFrame EnvironmentFrame;
struct EnvironmentFrame
{
  Symbol* key;
  Object* value;
  EnvironmentFrame* next;
};

EnvironmentFrame* EnvironmentFrame_create(Symbol* key, Object* value, EnvironmentFrame* next)
{
  EnvironmentFrame* result = malloc(sizeof(EnvironmentFrame));
  result->key = key;
  result->value = value;
  result->next = next;
  return result;
}

struct Environment;
typedef struct Environment Environment;
struct Environment
{
  EnvironmentFrame* first;
};

void Environment_initialize(Environment* e)
{
  e->first = NULL;
}

Environment* Environment_create()
{
  Environment* result = malloc(sizeof(Environment));
  Environment_initialize(result);
  return result;
}

void Environment_destroy(Environment* e)
{
  EnvironmentFrame* current = e->first;

  while(current != NULL)
  {
    EnvironmentFrame* previous = current;
    current = current->next;
    free(previous);
  }

  free(e);
}

Object* Environment_get(Environment* self, Symbol* key)
{
  EnvironmentFrame* current;

  for(current = self->first; current != NULL; current = current->next)
  {
    if(Symbol_isEqual(current->key, key))
    {
      return current->value;
    }
  }

  return NULL;
}

void Environment_set(Environment* self, Symbol* key, Object* value)
{
  self->first = EnvironmentFrame_create(key, value, self->first);
}

#endif
