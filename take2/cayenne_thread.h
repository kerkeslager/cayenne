#ifndef CAYENNE_THREAD_H
#define CAYENNE_THREAD_H

#include <stdbool.h>

#include "instruction.h"

struct CayenneThread;
typedef struct CayenneThread CayenneThread;

void CayenneThread_init(CayenneThread*, InstructionCode*);
void CayenneThread_destroy(CayenneThread*);

struct CayenneThreadProgressReport;
typedef struct CayenneThreadProgressReport CayenneThreadProgressReport;

CayenneThreadProgressReport CayenneThread_progress(CayenneThread*);

bool CayenneThreadProgressReport_isHalted(CayenneThreadProgressReport*);

#endif

