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

  // Append the node
  while(!__sync_bool_compare_and_swap(&(queue->tail->next), NULL, node));

  // Reuse the node variable to update the tail
  while((node = queue->tail)->next != NULL)
  {
    __sync_bool_compare_and_swap(&(queue->tail), node, node->next);
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
    previous = self->head;
    while(MPMCQueue_isEmpty(self))
    {
      pthread_yield();
    }

    /* This can't be done outside the loop, because another thread might free
    previous->next before this thread gets there. */
    result = previous->next->item;
  }
  while(!__sync_bool_compare_and_swap(&(queue->head), previous, previous->next));

  free(previous);
  return item;
}

#endif
