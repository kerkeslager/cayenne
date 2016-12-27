#ifndef PROGRAM_T
#define PROGRAM_T

#include "program.h"

#include "cayenne_thread_mpmc_queue.t"

struct Program {
  uint8_t running_pthread_count;
  CayenneThreadMPMCQueue cayenneThreadQueue;
};

#endif
