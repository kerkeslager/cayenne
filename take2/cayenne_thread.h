#ifndef CAYENNE_THREAD_H
#define CAYENNE_THREAD_H

#include <stdbool.h>

struct CayenneThread;
typedef struct CayenneThread CayenneThread;

struct CayenneThreadProgressReport;
typedef struct CayenneThreadProgressReport CayenneThreadProgressReport;

CayenneThreadProgressReport CayenneThread_progress(CayenneThread*);

bool CayenneThreadProgressReport_isHalted(CayenneThreadProgressReport*);

#endif

