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

#include <assert.h>
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
  void* item;
  MPSCQueueNode* next;
};

void initMPSCQueue(MPSCQueue* queue)
{
  MPSCQueueNode* fake = malloc(sizeof(MPSCQueueNode));

  fake->next = NULL;

  queue->head = fake;
  queue->tail = fake;
}

void enqueue(MPSCQueue* queue, void* item)
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

void* dequeue(MPSCQueue* queue, void* defaultResult)
{
  if(queue->head == queue->tail) return defaultResult;

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
