#ifndef INSTRUCTION_T
#define INSTRUCTION_T

#include "instruction.h"

typedef CayenneThreadProgressReport (*Instruction)(CayenneThread*);
Instruction InstructionCode_toInstruction(InstructionCode*);

#endif
