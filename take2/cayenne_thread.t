#ifndef CAYENNE_THREAD_T
#define CAYENNE_THREAD_T

#include "cayenne_thread.h"

struct CayenneThread
{
  char id;
  int state;
};

struct CayenneThreadProgressReport
{
  bool halted;
};

#endif

