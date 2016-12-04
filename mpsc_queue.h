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

#ifndef MPSC_QUEUE_H
#define MPSC_QUEUE_H

#include "object.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

struct MPSCQueue;
typedef struct MPSCQueue MPSCQueue;

struct MPSCQueueNode;
typedef struct MPSCQueueNode MPSCQueueNode;

struct MPSCQueue
{
  MPSCQueueNode* head;
  MPSCQueueNode* tail;
};

struct MPSCQueueNode
{
  Object* item;
  MPSCQueueNode* next;
};

void MPSCQueue_initialize(MPSCQueue* self)
{
  MPSCQueueNode* fake = malloc(sizeof(MPSCQueueNode));

  fake->next = NULL;

  self->head = fake;
  self->tail = fake;
}

void MPSCQueue_enqueue(MPSCQueue* queue, Object* item)
{
  // Create the node
  MPSCQueueNode* node = malloc(sizeof(MPSCQueueNode));
  node->item = item;
  node->next = NULL;

  // Append the node to the linked list, but the tail isn't updated yet
  while(!__sync_bool_compare_and_swap(&(queue->tail->next), NULL, node));

  /* Reuse the node variable to update the tail. Note that the algorithm used
  has some implications:
  1.  Any producer thread can update the tail, and there's no guarantee it will
      be this thread that updates the tail. This thread will be blocked on this
      loop until the tail is updated such that queue->tail->next == NULL (this
      means the tail has been updated to include the added node for every
      producer that has added a node).
  2.  Since the queue is considered empty when head == tail, if the queue was
      previously empty, the consumer will be blocked until the tail updates.
  3.  Since all the producers are racing to advance the tail as far as
      possible, the tail will likely be advanced to the end (unblocking the
      producers, and even more likely to advance one node (unblocking the 
      consumer). */
  while((node = queue->tail)->next != NULL)
  {
    /* TODO Is it a problem that node may have been freed by the consumer?
    Very unlikely but still... */
    __sync_bool_compare_and_swap(&(queue->tail), node, node->next);
  }
}

bool MPSCQueue_isEmpty(MPSCQueue* self)
{
  return self->head == self->tail;
}

Object* MPSCQueue_dequeue(MPSCQueue* queue)
{
  assert(!MPSCQueue_isEmpty(queue));
  assert(queue->head->next != NULL);

  MPSCQueueNode* previous = queue->head;
  queue->head = previous->next;
  free(previous);
  return queue->head->item;
}

void MPSCQueue_destroy(MPSCQueue* self)
{
  MPSCQueueNode* node = self->head;

  while(node != NULL)
  {
    MPSCQueueNode* temp = node;
    node = temp->next;
    free(temp);
  }
}

#endif
