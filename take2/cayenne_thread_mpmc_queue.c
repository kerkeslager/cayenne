#ifndef CAYENNE_THREAD_MPMC_QUEUE_C
#define CAYENNE_THREAD_MPMC_QUEUE_C

#include "cayenne_thread_mpmc_queue.t"

void CayenneThreadMPMCQueue_init(CayenneThreadMPMCQueue* self)
{
  MPMCQueue_init(&(self->internal));
}

void CayenneThreadMPMCQueue_destroy(CayenneThreadMPMCQueue* self)
{
  MPMCQueue_destroy(&(self->internal));
}

void CayenneThreadMPMCQueue_enqueue(CayenneThreadMPMCQueue* self, CayenneThread* item)
{
  MPMCQueue_enqueue(&(self->internal), (void*)item);
}

CayenneThread* CayenneThreadMPMCQueue_dequeue(CayenneThreadMPMCQueue* self)
{
  return (CayenneThread*)MPMCQueue_dequeue(&(self->internal));
}

#endif
