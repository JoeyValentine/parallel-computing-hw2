#include "tpool.h"

void 
init_thread_pool(int n_thread)
{
    pthread_cond_init(&is_q_empty, NULL);
    pthread_mutex_init(&q_mutex, NULL);
    threads = malloc(n_thread * sizeof(pthread_t));
}

