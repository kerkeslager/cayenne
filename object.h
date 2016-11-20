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

#include <stdbool.h>
#include <stdint.h>

enum Type
{
  TYPE_BOOLEAN,
  TYPE_INTEGER
};
typedef enum Type Type;

union Instance;
typedef union Instance Instance;
union Instance
{
  bool boolean;
  int32_t integer;
};

struct Object;
typedef struct Object Object;
struct Object
{
  Type type;
  Instance instance;
};

#endif
