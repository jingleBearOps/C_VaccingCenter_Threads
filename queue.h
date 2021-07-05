#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct PersonInfo PersonInfo_t;
struct PersonInfo {
    int          id;
    int          arrival_time;
    int          service_time;
    PersonInfo_t *next;
};

typedef struct Queue Queue_t;
struct Queue {
    PersonInfo_t *start;
    PersonInfo_t *end;
};

Queue_t *init_queue();
int is_empty(Queue_t *);

PersonInfo_t *new_person();

void enqueue(Queue_t *, PersonInfo_t *);
PersonInfo_t *dequeue(Queue_t *);
int queue_size(Queue_t *);

void deallocate(void *);

#endif
