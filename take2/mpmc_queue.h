#ifndef MPMC_QUEUE_H
#define MPMC_QUEUE_H

struct MPMCQueue;
typedef struct MPMCQueue MPMCQueue;

void MPMCQueue_init(MPMCQueue*);
void MPMCQueue_destroy(MPMCQueue*);

void MPMCQueue_enqueue(MPMCQueue*, void*);
void* MPMCQueue_dequeue(MPMCQueue*);

#endif
