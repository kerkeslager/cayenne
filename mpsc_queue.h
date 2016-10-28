#ifndef MPSC_QUEUE
#define MPSC_QUEUE

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
