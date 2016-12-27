#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "cayenne_thread.h"

#include <stdint.h>

typedef uint8_t InstructionCode;
typedef CayenneThreadProgressReport (*Instruction)(CayenneThread*);

#define NOOP 0
#define HALT 1
#define PUSH_10 10
#define PUSH_20 20
#define PRINT 9

Instruction InstructionCode_toInstruction(InstructionCode*);

#endif
