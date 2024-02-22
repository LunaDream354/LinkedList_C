#include <stdio.h>
#include <string.h>

#include "linked_list.h"
int list_text[] = {2, 3, 1, 6, 8, 10, 0};
bool sort(void *item1, void *item2);
int main(void) {
    Linked_head *list = linked_create();
    for (size_t i = 0; i < sizeof(list_text) / sizeof(list_text[0]); i++)
        linked_append(list, &list_text[i]);
    linked_sort(&list, sort);
    for (Linked_node *current = list->node; current != NULL;
         current = current->next)
        printf("%d\n", *(int *)current->data);
    return 0;
}

bool sort(void *item1, void *item2) {
    return (*(int *)item1) < (*(int *)item2);
}