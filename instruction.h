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

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "symbol.h"

#include <stdint.h>

enum Opcode
{
  OPCODE_HELLO,
  OPCODE_NOOP,
  OPCODE_HALT,
  OPCODE_FORK,
  OPCODE_DROP,
  OPCODE_SWAP,
  OPCODE_LOAD,
  OPCODE_STORE,
  OPCODE_CALL,
  OPCODE_RETURN,
  // OPCODE_SEND,
  // OPCODE_FORK,
  OPCODE_RECEIVE// ,
  // OPCODE_TAILCALL,
  // OPCODE_CONS,
  // OPCODE_CAR,
  // OPCODE_CDR
};
typedef enum Opcode Opcode;

struct Instruction;
typedef struct Instruction Instruction;
struct Instruction
{
  Opcode opcode;
  Symbol* symbol;
};


#endif
