#ifndef DEQUE_H_
#define DEQUE_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct DElem {
    int val;       // value pushed
    void *next;    // next DElem in deque
    void *prev;    // previous DElem in deque
    bool is_empty; // only used for assertions right now. I might
                   // remove it at some point...
} DElem;

typedef struct Deque {
    DElem *buff;       // ptr to allocated memory
    DElem *head;       // ptr to head (front)
    DElem *back;       // ptr to back
    DElem *empty_head; // ptr to empty-head (1st emtpy DElem)
    DElem *empty_back; // ptr to empty-back (last empty DElem)
    size_t count;      // current number of pushed values
    size_t capacity;   // number of allocated DElems in buff
} Deque;

// prototypes
Deque deque_create(size_t n_elems);
void deque_destroy(Deque *deque);

void deque_push_front(Deque *deque, int val);
int deque_pop_front(Deque *deque);

void deque_push_back(Deque *deque, int val);
int deque_pop_back(Deque *deque);

int deque_front(Deque deque);
int deque_back(Deque deque);

void deque_print(Deque deque);

// implementations
#ifdef DEQUE_IMPLEMENTATION

#include <assert.h>

#define array_len(x) sizeof(x) / sizeof(*(x))

Deque deque_create(size_t n_elems)
{
    Deque deque;
    deque.buff = malloc(n_elems * sizeof *deque.buff);
    if (deque.buff == NULL) {
        fprintf(stderr, "Error: ['deque_create'] deque couldn't be allocated\n");
        exit(1);
    }
    deque.head       = NULL;
    deque.back       = NULL;
    deque.count      = 0;
    deque.capacity   = n_elems;
    deque.empty_head = deque.buff;
    deque.empty_back = deque.buff + deque.capacity - 1;

    DElem *next;
    DElem *prev;
    for (size_t i = 0; i < deque.capacity; ++i) {
        if (i < deque.capacity - 1) { // all but last elem
            next = deque.buff + i + 1;
        } else {
            next = NULL;
        }
        
        if (i > 0) { // all but first elem
            prev = deque.buff + i - 1;
        } else {
            prev = NULL;
        }
        
        deque.buff[i] = (DElem) {
            .val = 0,
            .next = next,
            .prev = prev,
            .is_empty = true
        };
    }

    return deque;
}

void deque_destroy(Deque *deque)
{
    if (deque->buff != NULL) {
        free(deque->buff);
    }
}

void __deque_grow(Deque *deque)
{
    size_t realloc_inc_rate = 2;
    size_t new_capacity     = realloc_inc_rate * deque->capacity;
    if ((deque->buff = realloc(deque->buff, new_capacity * sizeof(*deque->buff))) == NULL) {
        printf("Error: ['__deque_grow'] deque buff couldn't be reallocated");
        exit(1);
    }
    
    // initialize new elems as in deque_create
    DElem *next;
    DElem *prev;
    for (size_t i = deque->capacity; i < new_capacity; ++i) {
        if (i < new_capacity - 1) { // all but last elem
            next = deque->buff + i + 1;
        } else {
            next = NULL;
        }
        
        if (i > deque->capacity) { // all but first elem
            prev = deque->buff + i - 1;
        } else {
            prev = NULL;
        }
        
        deque->buff[i] = (DElem) {
            .val = 0,
            .next = next,
            .prev = prev,
            .is_empty = true
        };
    }
    
    deque->empty_head = deque->buff + deque->capacity;
    deque->empty_back = deque->buff + new_capacity - 1;
    deque->capacity   = new_capacity;
}

void __deque_shrink(Deque *deque)
{
    int temp[deque->count];
    DElem *next_head = deque->head;
    for (size_t i = 0; i < deque->count && next_head != NULL; ++i) {
        temp[i] = next_head->val;
        next_head = next_head->next;
    }
    
    size_t realloc_shrink_rate = 2;
    size_t new_capacity = deque->capacity / realloc_shrink_rate;
    if ((deque->buff = realloc(deque->buff, new_capacity * sizeof(*deque->buff))) == NULL) {
        printf("Error: ['__deque_shrink'] deque buff couldn't be reallocated");
        exit(1);
    }
    
    deque->head       = NULL;
    deque->back       = NULL;
    deque->count      = 0;
    deque->capacity   = new_capacity;
    deque->empty_head = deque->buff;
    deque->empty_back = deque->buff + deque->capacity - 1;

    DElem *next;
    DElem *prev;
    for (size_t i = 0; i < deque->capacity; ++i) {
        if (i < deque->capacity - 1) { // all but last elem
            next = deque->buff + i + 1;
        } else {
            next = NULL;
        }
        
        if (i > 0) { // all but first elem
            prev = deque->buff + i - 1;
        } else {
            prev = NULL;
        }
        
        deque->buff[i] = (DElem) {
            .val = 0,
            .next = next,
            .prev = prev,
            .is_empty = true
        };
    }

    for (size_t i = 0; i < array_len(temp); ++i) {
        deque_push_back(deque, temp[i]);
    }
    
}

int deque_front(Deque deque)
{
    if (deque.count <= 0) {
        printf("Error: ['deque_front'] deque is emtpy");
        exit(1);
    }
    return deque.head->val;
}

int deque_back(Deque deque)
{
    if (deque.count <= 0) {
        printf("Error: ['deque_back'] deque is emtpy");
        exit(1);
    }
    return deque.back->val;
}

void deque_push_front(Deque *deque, int val)
{
    if (deque->count == deque->capacity) __deque_grow(deque);
    
    if (deque->count > 0) { // if deque not empty
        assert(deque->empty_head->is_empty && "AssertionError: ['deque_push_front'] pushing to a non-empty DElem");
        DElem *next_empty = deque->empty_head->next;
        *deque->empty_head = (DElem) {
            .val = val,
            .next = deque->head,
            .prev = NULL,
            .is_empty = false
        };
        deque->head->prev = deque->empty_head; // prev head to point to new head loc
        deque->head       = deque->empty_head; // set new head loc
        deque->empty_head = next_empty;        // update empty
        deque->count++;
    } else { // if deque is empty
        assert(deque->empty_head->is_empty && "AssertionError: ['deque_push_front'] pushing to a non-empty DElem");
        DElem *next_empty = deque->empty_head->next;
        *deque->empty_head = (DElem) {
            .val  = val,
            .next = NULL,
            .prev = NULL,
            .is_empty = false
        };
        deque->head = deque->empty_head;
        deque->back = deque->empty_head;
        deque->empty_head = next_empty;
        deque->count++;
    }
    if (deque->count == deque->capacity) deque->empty_back = NULL; // if pushed elem filled deque->buff
}

void deque_push_back(Deque *deque, int val)
{
    if (deque->count == deque->capacity) __deque_grow(deque);
    
    if (deque->count > 0) { // if deque not empty
        assert(deque->empty_head->is_empty && "AssertionError: ['deque_push_front'] pushing to a non-empty DElem");
        DElem *next_empty = deque->empty_head->next;
        *deque->empty_head = (DElem) {
            .val  = val,
            .next = NULL,
            .prev = deque->back,
            .is_empty = false
        };
        deque->back->next = deque->empty_head; // prev back to point to new back loc
        deque->back       = deque->empty_head; // set new backloc
        deque->empty_head = next_empty;        // update empty
        deque->count++;
    } else { // if deque is empty
        assert(deque->empty_head->is_empty && "AssertionError: ['deque_push_front'] pushing to a non-empty DElem");
        DElem *next_empty = deque->empty_head->next;
        *deque->empty_head = (DElem) {
            .val  = val,
            .next = NULL,
            .prev = NULL,
            .is_empty = false
        };
        deque->head = deque->empty_head;
        deque->back = deque->empty_head;
        deque->empty_head = next_empty;
        deque->count++;
    }
    if (deque->count == deque->capacity) deque->empty_back = NULL; // if pushed elem filled deque->buff
}

int deque_pop_front(Deque *deque)
{
    if (deque->head == NULL) {
        fprintf(stderr, "Error: ['deque_pop_front'] deque is empty, can't pop\n");
        exit(1);
    }

    int val = deque->head->val;
    DElem *next_empty = deque->head;

    if (deque->head == deque->back) deque->back = NULL; // if just one elem (empty is handled above)
    deque->head = deque->head->next;
    if (deque->head != NULL) deque->head->prev = NULL;
    deque->count--;

    *next_empty = (DElem) {
        .val  = 0,
        .next = deque->empty_head,
        .prev = NULL,
        .is_empty = true
    };
    deque->empty_head->prev = next_empty;
    deque->empty_head       = next_empty;

    if (deque->count <= deque->capacity / 4) __deque_shrink(deque);
    
    return val;
}

int deque_pop_back(Deque *deque)
{
    if (deque->back == NULL) {
        fprintf(stderr, "Error: ['deque_pop_back'] deque is empty, can't pop\n");
        exit(1);
    }

    int val = deque->back->val;
    DElem *next_empty = deque->back;
    
    if (deque->head == deque->back) deque->head = NULL;
    deque->back = deque->back->prev;
    if (deque->back != NULL) deque->back->next = NULL;
    deque->count--;

    *next_empty = (DElem) {
        .val  = 0,
        .next = deque->empty_head,
        .prev = NULL,
        .is_empty = true
    };
    deque->empty_head->prev = next_empty;
    deque->empty_head       = next_empty;

    if (deque->count <= deque->capacity / 4) __deque_shrink(deque);
    
    return val;
}

void deque_print(Deque deque)
{
    DElem *next = deque.head;
    putc('[', stdout);
    for (size_t i = 0; i < deque.count; i++) {
        printf("%d", next->val);
        next = next->next;
        if (next != NULL) putc(' ', stdout);
    }
    puts("]\n");
}

#endif // DEQUE_IMPLEMENTATION
#endif // DEQUE_H_
