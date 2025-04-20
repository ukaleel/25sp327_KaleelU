
#define _GNU_SOURCE

#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "common.h"
#include "queue_internal.h"


queue_t* queue_init(pthread_mutex_t* mutex, pthread_cond_t* cond_var){
    // malloc a queue

    queue_t* que = malloc(sizeof(queue_t));
    //populate a queue
    que->header = NULL;
    que->tail = NULL;
    que->size = 0;
    que->isclosed = false;
    que->mutex = mutex;
    que->cond_var = cond_var;
    return que;

    

}


void queue_enqueue(queue_t* que, void* data) {
    // Allocate a new node
    queue_node_t* node = malloc(sizeof(queue_node_t));
    node->data = data;
    node->next = NULL;

    // Begin critical section
    LOCK_MTX(que->mutex);

    // Add the node to the queue
    if (que->header == NULL) {
        que->header = node;
        que->tail = node;
    } else {
        que->tail->next = node;
        que->tail = node; // Ensure tail is updated
        que->tail->next = NULL;
    }
    que->size++;

    pthread_cond_signal(que->cond_var);

    // End critical section
    UNLOCK_MTX(que->mutex);
}

void* queue_dequeue(queue_t* que){
    //begin CS
    LOCK_MTX(que->mutex);
    //manipulate queue

    while (que->size == 0 && !que->isclosed)
    {
        pthread_cond_wait(que->cond_var, que->mutex);
    }

    if (que->size == 0 && que->isclosed)
    {
        UNLOCK_MTX(que->mutex);
        return NULL;
    }

    queue_node_t* node = que->header;
    que->header = que->header->next;
    que->size--;
    //end CS
    UNLOCK_MTX(que->mutex);

    void * data = node->data;
    free(node);
    return data;

}

struct node* queue_front(queue_t* que){
    //begin CS
    LOCK_MTX(que->mutex);
    //manipulate queue
    queue_node_t* node = que->header;
    //end CS
    UNLOCK_MTX(que->mutex);
    return node->data;
}

int queue_size(queue_t* que){
    //begin CS
    LOCK_MTX(que->mutex);
    //manipulate queue
    int length = que->size;
    //end CS
    UNLOCK_MTX(que->mutex);
    return length;
}

bool queue_isempty(queue_t* que){
    //begin CS
    LOCK_MTX(que->mutex);
    //manipulate queue
    bool is_empty = (que->size == 0);
    //end CS
    UNLOCK_MTX(que->mutex);
    return is_empty;
}

bool queue_isclose(queue_t* que){
    //begin CS
    LOCK_MTX(que->mutex);
    bool is_closed = que->isclosed;
    //end CS
    UNLOCK_MTX(que->mutex);
    return is_closed;
}

void queue_destroy(queue_t* que){

    //frees all nodes in entirety!
    LOCK_MTX(que->mutex);
    queue_node_t* current = que->header;
    while (current != NULL)
    {
        queue_node_t* next = current->next;
        free(current);
        current = next;
    }
    UNLOCK_MTX(que->mutex);
    free(que);

}


void queue_close(queue_t* que){
    //begin CS
    LOCK_MTX(que->mutex);
    que->isclosed = true;
    pthread_cond_broadcast(que->cond_var);

    //end CS
    UNLOCK_MTX(que->mutex);
}