#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdbool.h>
#include <stdlib.h>
typedef struct Linked_node {
    void *data;
    struct Linked_node *const next;
} Linked_node;
typedef struct Linked_head {
    size_t size;
    Linked_node *node;
} Linked_head;

Linked_head *linked_create(void *data, bool initialized);
Linked_head *linked_push(Linked_head *list, void *data);
Linked_head *linked_add_at(Linked_head *list, void *data, size_t position);
void *linked_pop(Linked_head *list);
void *linked_remove_at(Linked_head *list, size_t position);
void *linked_get_at(Linked_head *list, size_t position);
void linked_delete(Linked_head **list);
Linked_node *linked_get_node_at(Linked_head *list, size_t position);
void linked_sort(Linked_head **list, bool (*func)(void *, void *));
bool linked_search(Linked_head *list, size_t *positions, size_t count,
                   void *search, bool (*searchFunc)(void *, void *));
bool linked_is_node_valid(Linked_node *node);

#endif