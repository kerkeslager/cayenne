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

#ifndef MPMC_QUEUE_H
#define MPMC_QUEUE_H

#include <pthread.h>

#include "green_thread.h"

struct MPMCQueue;
typedef struct MPMCQueue MPMCQueue;

struct MPMCQueueNode;
typedef struct MPMCQueueNode MPMCQueueNode;

struct MPMCQueue
{
  MPMCQueueNode* head;
  MPMCQueueNode* tail;
};

struct MPMCQueueNode
{
  GreenThread* item;
  MPMCQueueNode* next;
};

void MPMCQueue_initialize(MPMCQueue* self)
{
  MPMCQueueNode* fake = malloc(sizeof(MPMCQueueNode));

  fake->next = NULL;
  
  self->head = fake;
  self->tail = fake;
}

void MPMCQueue_enqueue(MPMCQueue* self, GreenThread* item)
{
  // Create the node
  MPMCQueueNode* node = malloc(sizeof(MPMCQueueNode));
  node->item = item;
  node->next = NULL;

  // Append the node to the linked list, but the tail isn't updated yet
  while(!__sync_bool_compare_and_swap(&(self->tail->next), NULL, node));

  /* Reuse the node variable to update the tail. Note that the algorithm used
  has some implications:
  1.  Any producer thread can update the tail, and there's no guarantee it will
      be this thread that updates the tail. This thread will be blocked on this
      loop until the tail is updated such that queue->tail->next == NULL (this
      means the tail has been updated to include the added node for every
      producer that has added a node).
  2.  Since the queue is considered empty when head == tail, if the queue was
      previously empty, the consumers will be blocked until the tail updates.
  3.  Since all the producers are racing to advance the tail as far as
      possible, the tail will likely be advanced to the end (unblocking the
      producers, and even more likely to advance one node (unblocking one 
      of the consumers). */
  while((node = self->tail)->next != NULL)
  {
    /* TODO Is it a problem that node may have been freed by a consumer?
    Very unlikely but still... */
    __sync_bool_compare_and_swap(&(self->tail), node, node->next);
  }
}

bool MPMCQueue_isEmpty(MPMCQueue* self)
{
  return self->head == self->tail;
}

GreenThread* MPMCQueue_dequeue(MPMCQueue* self)
{
  MPMCQueueNode* previous;
  GreenThread* result;

  do
  {
    /* This has to be done before we yield if the queue is empty because
    another consumer might update the head so that the queue isn't empty after
    the empty check, but before previous is set. */
    previous = self->head;

    /* Yield if the queue is empty. */
    while(MPMCQueue_isEmpty(self))
    {
      pthread_yield();
    }

    /* This can't be done outside the loop, because another consumer might free
    previous before this thread gets there. */
    result = previous->next->item;

    /* TODO Is it a problem that previous may have been freed by another consumer? */
  }
  while(!__sync_bool_compare_and_swap(&(self->head), previous, previous->next));

  free(previous);
  return result;
}

#endif
