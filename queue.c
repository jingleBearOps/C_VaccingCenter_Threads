/*
 * queue.c
 *
 * Based on the linkedlist implementation approach described in 
 * "The Practice of Programming" by Kernighan and Pike 
 * (Addison-Wesley, 1999).
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/*
 * Wrapper around malloc() -- ends the program if malloc() fails
 * (i.e., eliminates the need of an "if" statement around each
 * call to malloc() in the main body of code).
 */
void *emalloc(size_t n) {
    void *p;

    p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "malloc of %zu bytes failed", n);
        exit(1);
    }

    return p;
}

/*
 * Initialize an empty Queue. 
 */
Queue_t *init_queue() {
    Queue_t* q = (Queue_t*) emalloc(sizeof(Queue_t));
    q->start = NULL;
    q->end = NULL;

    return q;
}

/*
 * Check if the queue is empty. 
 */
int is_empty(Queue_t* q){
    if(q->start == NULL) {
        return 1;
    }

    return 0;
}

/*
 * New linked-list node. Beware as the only field that gets a
 * specific value in this function is the "next" field; everything
 * else is uninitialized.
 */
PersonInfo_t *new_person() {
    PersonInfo_t *person;

    person = emalloc(sizeof(PersonInfo_t));
    person->next = NULL;

    return person;
}

/*
 * Wrapper to free allocated memory.
 */
void deallocate(void *t) {
    assert (t != NULL);
    free(t);
}

/*
 * At the end of this operation, the node passed in will be enqueued. 
 */
void enqueue(Queue_t *q, PersonInfo_t *person) {
    // person->next = NULL;

    if (is_empty(q)) {
        q->start = person;
        q->end = person;

    } else {
        q->end->next = person;
        q->end = person;
    }
}

/*
 * Returns and dequeues the start node from the queue.
 * Deallocate/free the Node after use.
 */
PersonInfo_t *dequeue(Queue_t *q) {
    PersonInfo_t *person;

    if (is_empty(q)) {
        return NULL;
    }
    person = q->start;
    q->start = person->next;

    if(q->start == NULL) {
        q->end = NULL;
    }

    return person;
}

/*
 * Return the number of nodes in the queue.
 */
int queue_size(Queue_t *q) {
    int size = 1;
    PersonInfo_t *p;

    if(is_empty(q)) {
        return 0;
    }

    for(p = q->start; p->next != NULL; p = p->next) {
        size++;
    }
    
    return size;
}
