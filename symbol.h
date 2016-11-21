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

#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>
#include <string.h>

struct Symbol;
typedef struct Symbol Symbol;
struct Symbol
{
  char* name;
};

bool Symbol_isEqual(Symbol* a, Symbol* b)
{
  return strcmp(a->name, b->name) == 0;
}

#endif
