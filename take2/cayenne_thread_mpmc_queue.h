#ifndef CAYENNE_THREAD_MPMC_QUEUE_H
#define CAYENNE_THREAD_MPMC_QUEUE_H

#include "cayenne_thread.h"

struct CayenneThreadMPMCQueue;
typedef struct CayenneThreadMPMCQueue CayenneThreadMPMCQueue;

void CayenneThreadMPMCQueue_init(CayenneThreadMPMCQueue*);
void CayenneThreadMPMCQueue_destroy(CayenneThreadMPMCQueue*);

void CayenneThreadMPMCQueue_enqueue(CayenneThreadMPMCQueue*, CayenneThread*);
CayenneThread* CayenneThreadMPMCQueue_dequeue(CayenneThreadMPMCQueue*);

#endif
