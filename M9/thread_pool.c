#include "thread_pool.h"
#include "worker_thread.h"
#include "common.h"
#include <stdlib.h>

struct thread_pool *thread_pool_init(queue_t *que, int num_threads)
{
    struct thread_pool *pool = malloc(sizeof(struct thread_pool));
    if (!pool){
        handle_error("thread_pool_init: malloc failed");
    }
    pool->next_thread_id = 1;
    pool->num_threads = num_threads;
    pool->que = que;
    
    for (int i = 0; i < MAX_WORKER_THREADS; i++){
        pool->threads[i] = NULL;
    }

    for (int i = 0; i < num_threads; i++){
        struct worker_thread_params *params = malloc(sizeof(struct worker_thread_params));
        if (!params){
            handle_error("thread_pool_init: malloc failed for worker_thread_params");
        }
        params->que = que;
        params->thread_id = pool->next_thread_id;
        pool->threads[i] = malloc(sizeof(struct worker_thread));

        if (!pool->threads[i]) {
            handle_error("thread_pool_init: malloc failed for worker_thread");
        }
        pool->threads[i]->thread_id = pool->next_thread_id;
        pool->threads[i]->thread = create_worker_thread(params);
        pool->next_thread_id++;
    }

    return pool;
}

void thread_pool_destroy(struct thread_pool *pool){
    for (int i = 0; i < pool->num_threads; i++) {
        if (pool->threads[i] != NULL){
            pthread_join(pool->threads[i]->thread, NULL);
            free(pool->threads[i]);
        }
    }
    free(pool);
}