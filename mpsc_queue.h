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

  // Append the node
  while(!__sync_bool_compare_and_swap(&(queue->tail->next), NULL, node));

  // Reuse the node variable to update the tail
  while((node = queue->tail)->next != NULL)
  {
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

void freeMPSCQueue(MPSCQueue* queue)
{
  MPSCQueueNode* node = queue->head;
  free(queue);

  while(node != NULL)
  {
    MPSCQueueNode* temp = node;
    node = temp->next;
    free(temp);
  }
}

#endif
