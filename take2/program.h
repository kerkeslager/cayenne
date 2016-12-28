#ifndef PROGRAM_H
#define PROGRAM_H

#include "instruction.h"

#include <stdint.h>

struct Program;
typedef struct Program Program;

Program Program_init(int8_t module_count, InstructionCode**);

void Program_run(Program*, uint8_t);

#endif
