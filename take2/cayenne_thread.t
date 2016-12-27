#ifndef CAYENNE_THREAD_T
#define CAYENNE_THREAD_T

#include "cayenne_thread.h"
#include "data_stack.t"

struct CayenneThread
{
  char id;
  DataStack dataStack;
};

struct CayenneThreadProgressReport
{
  bool halted;
};

#endif

