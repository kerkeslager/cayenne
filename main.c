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

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* fix yield on OS X */
#if defined(__APPLE__) || defined(__MACH__)
#define pthread_yield() pthread_yield_np()
#endif

#include "program.h"

Instruction* makeProgramInstructions()
{
  Instruction* instructions = malloc(sizeof(Instruction) * 2);
  instructions[0].opcode = OPCODE_HELLO;
  instructions[1].opcode = OPCODE_HALT;
  return instructions;
}

int main()
{
  printf(
    "cayenne 0.0.1\n"
    "Copyright (C) 2016 David Kerkeslager\n"
    "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
  );

  GreenThread* mainThread = malloc(sizeof(GreenThread));
  GreenThread_initialize(mainThread, makeProgramInstructions());

  Program program;
  Program_initialize(&program, mainThread);
  Program_run(&program);
  return 0;
}
