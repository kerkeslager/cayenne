#ifndef MPMC_QUEUE_T
#define MPMC_QUEUE_T

#include "mpmc_queue.h"

struct _MPMCQueueNode;
typedef struct _MPMCQueueNode _MPMCQueueNode;
struct _MPMCQueueNode
{
  void* item;
  _MPMCQueueNode* next;
};

struct MPMCQueue
{
  _MPMCQueueNode* head;
  _MPMCQueueNode* tail;
};

#endif
