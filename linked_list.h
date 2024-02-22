#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdbool.h>
#include <stdlib.h>

#define LINKED_ERROR_NO_MEMORY ENOMEM
#define LINKED_ERROR_LIST_NULL -2
#define LINKED_ERROR_OUT_BOUNDS EFAULT
#define LINKED_ERROR_PARAM_NULL EINVAL

typedef struct Linked_node {
    void *data;
    struct Linked_node *const next;
} Linked_node;
typedef struct Linked_head {
    const size_t size;
    Linked_node *const node;
} Linked_head;

Linked_head *linked_create(void);
Linked_head *linked_push(Linked_head *list, void *data);
Linked_head *linked_append(Linked_head *list, void *data);
Linked_head *linked_add_at(Linked_head *list, void *data, size_t position);

void *linked_pop(Linked_head *list);
void *linked_remove_at(Linked_head *list, size_t position);
void *linked_get_at(Linked_head *list, size_t position);
void linked_delete(Linked_head **list);
Linked_node *linked_get_node_at(Linked_head *list, size_t position);
Linked_head *linked_sort(Linked_head **list, bool (*organizer)(void *, void *));
long linked_search(Linked_head *list, Linked_head **positions, void *search,
                   bool (*searchFunc)(void *, void *));
Linked_head *linked_merge(Linked_head *list1, Linked_head *list2);
#endif