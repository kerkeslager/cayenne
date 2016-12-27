#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdint.h>

struct Program;
typedef struct Program Program;

Program Program_init();

void Program_run(Program*, uint8_t);

#endif
