#ifndef __TPOOL__
#define __TPOOL__

#include <pthread.h>

#include "queue.h"

extern pthread_t *threads;
extern pthread_cond_t is_q_empty;
extern pthread_mutex_t q_mutex;

void init_thread_pool(int n_thread);

#endif
