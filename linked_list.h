#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdbool.h>
#include <stdlib.h>

#define EFAULT

typedef struct Linked_node {
    void *data;
    struct Linked_node *const next;
} Linked_node;
typedef struct Linked_head {
    const size_t size;
    Linked_node *const node;
} Linked_head;

bool linked_create(Linked_head **list);
bool linked_push(const Linked_head *list, const void *data);
bool linked_add_at(const Linked_head *list, const void *data, size_t position);
bool linked_append(const Linked_head *list,const void *data);
bool linked_pop(const Linked_head *list, void **data);
bool linked_remove_at(const Linked_head *list, size_t position, void **data);
bool linked_get_at(const Linked_head *list, size_t position, void **data);
void linked_delete(Linked_head **list);
bool linked_get_node_at(const Linked_head *list, size_t position,Linked_node **node);
Linked_head *linked_sort(Linked_head **list, bool (*organizer)(const void *,const void *));
long linked_search(const Linked_head *list, Linked_head **positions,
                   const void *search, bool (*searchFunc)(const void *,const void *));
Linked_head *linked_merge(const Linked_head *list1,
                                const Linked_head *list2);
#endif