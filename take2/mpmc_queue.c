#ifndef MPMC_QUEUE_C
#define MPMC_QUEUE_C

#include "mpmc_queue.t"

#include <stdbool.h>

void MPMCQueue_init(MPMCQueue* self)
{
  _MPMCQueueNode* fake = malloc(sizeof(_MPMCQueueNode));

  fake->next = NULL;
  
  self->head = fake;
  self->tail = fake;
}

void MPMCQueue_enqueue(MPMCQueue* self, void* item)
{
  // Create the node
  _MPMCQueueNode* node = malloc(sizeof(_MPMCQueueNode));
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

void* MPMCQueue_dequeue(MPMCQueue* self)
{
  _MPMCQueueNode* previous;
  void* result;

  do
  {
    /* This has to be done before we exit if the queue is empty because
    another consumer might update the head so that the queue isn't empty after
    the empty check, but before previous is set. */
    previous = self->head;

    /* Exit if the queue is empty. */
    if(MPMCQueue_isEmpty(self)) return NULL;

    /* This can't be done outside the loop, because another consumer might free
    previous (which is previous->next for this thread) before this thread gets there. */
    result = previous->next->item;

    /* TODO Is it a problem that previous may have been freed by another consumer? */
  }
  while(!__sync_bool_compare_and_swap(&(self->head), previous, previous->next));

  free(previous);
  return result;
}

void MPMCQueue_destroy(MPMCQueue* self)
{
  _MPMCQueueNode* node = self->head;

  while(node != NULL)
  {
    _MPMCQueueNode* temp = node;
    node = temp->next;
    free(temp);
  }
}

#endif
