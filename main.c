/* This file is part of Cayenne.

Cayenne is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Cayenne is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Cayenne.  If not, see <http://www.gnu.org/licenses/>. */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "green_thread.h"

#define PTHREAD_COUNT 10

void* driverLoop(void* arg)
{
  int threadId = *((int*) arg);
  printf("Greetings from thread %d!\n", threadId);
  return NULL;
}

void startPThreads()
{
  pthread_t threads[PTHREAD_COUNT];
  int thread_args[PTHREAD_COUNT];
  int rc, i;

  /* spawn the threads */
  for(i = 0; i < PTHREAD_COUNT; i++)
  {
    thread_args[i] = i;
    rc = pthread_create(&threads[i], NULL, driverLoop, (void*) &thread_args[i]);
  }

  /* wait for threads to finish */
  for (i = 0; i < PTHREAD_COUNT; i++)
  {
    rc = pthread_join(threads[i], NULL);
  }
}

int main()
{
  startPThreads();
  return 0;
}
