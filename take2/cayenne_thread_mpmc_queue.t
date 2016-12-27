#ifndef CAYENNE_THREAD_MPMC_QUEUE_T
#define CAYENNE_THREAD_MPMC_QUEUE_T

#include "mpmc_queue.t"

#include "cayenne_thread_mpmc_queue.h"

struct CayenneThreadMPMCQueue
{
  MPMCQueue internal;
};

#endif
