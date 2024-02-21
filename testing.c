#include <stdio.h>
#include <string.h>

#include "linked_list.h"
char *list_text[] = {"aweawe", "XDXDXDXD", "LULULULU"};
bool search(void *item1, void *item2);
int main(void) {
    Linked_head *list = linked_create(NULL, false);
    for (size_t i = 0; i < sizeof(list_text) / sizeof(list_text[0]); i++)
        linked_append(list, list_text[i]);
    linked_sort(list,search);
    for (Linked_node *current = list->node; current != NULL;
         current = current->next)
        printf("%s\n", current->data);
    return 0;
}

bool search(void *item1, void *item2) { 
    return strcmp(item1, item2) > 0; 
}