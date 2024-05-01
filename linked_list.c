#include "linked_list.h"

#include <errno.h>
#include <stdint.h>
extern int errno;

void linked_node_set_next(const Linked_node *node, Linked_node *next);
Linked_node *linked_node_create(const void *data);
Linked_head *linked_sort_internal(Linked_head *list,
                                  bool (*organizer)(const void *, const void *));
void linked_list_size_edit(const Linked_head *list, size_t value);
void linked_list_node_set(const Linked_head *list, Linked_node *node);
Linked_head *linked_merge_sort(Linked_head **list_left,
                               Linked_head **list_right,
                               bool (*organizer)(const void *, const void *));

bool linked_create(Linked_head **list) {
    if (list == NULL || *list != NULL) {
        errno = EINVAL;
        return false;
    }
    *list = (Linked_head *)calloc(1, sizeof(Linked_head));
    if (*list == NULL) {
        errno = ENOMEM;
        return false;
    }
    return true;
}
bool linked_push(const Linked_head *list, const void *data) {
    if (list == NULL) {
        errno = EINVAL;
        return false;
    }
    Linked_node *node = linked_node_create(data);
    if (node == NULL) {
        return false;
    }
    if (list->size) {
        linked_node_set_next(node, list->node);
    }
    linked_list_node_set(list, node);
    linked_list_size_edit(list, list->size + 1);
    return true;
}

bool linked_add_at(const Linked_head *list, const void *data, size_t position) {
    if (list == NULL) {
        errno = EINVAL;
        return false;
    }
    if (!position) return linked_push(list, data);
    if (position == list->size) return linked_append(list, data);
    if (position > list->size) {
        errno = EFAULT;
        return false;
    }
    Linked_node *node_current = list->node;
    Linked_node *node_next = list->node;
    for (size_t i = 0; i < position; i++) {
        node_current = node_next;
        node_next = node_next->next;
    }
    Linked_node *node = linked_node_create(data);
    if (node == NULL) return false;
    linked_node_set_next(node_current, node);
    linked_node_set_next(node, node_next);
    return true;
}

bool linked_append(const Linked_head *list, const void *data) {
    if (list == NULL) {
        errno = EINVAL;
        return false;
    }

    Linked_node *crawler = NULL;
    if(list->size != 0)
        for (crawler = list->node; crawler->next != NULL;
            crawler = crawler->next);
    Linked_node *node = linked_node_create(data);
    if (node == NULL) return false;
    if (crawler == NULL)
        linked_list_node_set(list, node);
    else
        linked_node_set_next(crawler, node);

    linked_list_size_edit(list, list->size + 1);
    return true;
}

bool linked_pop(const Linked_head *list, void **data) {
    if (list == NULL) {
        errno = EINVAL;
        return false;
    }
    if (!list->size) return NULL;
    Linked_node *node_last = list->node;
    Linked_node *node_penult = list->node;
    while (node_last->next != NULL) {
        node_penult = node_last;
        node_last = node_last->next;
    }
    if (data != NULL) *data = node_last->data;
    linked_node_set_next(node_penult, NULL);
    free(node_last);

    linked_list_size_edit(list, list->size - 1);

    return true;
}
bool linked_remove_at(const Linked_head *list, size_t position, void **data) {
    if (list == NULL) {
        errno = EINVAL;
        return NULL;
    }
    if (position >= list->size) {
        errno = EFAULT;
        return NULL;
    }
    if (!list->size) return NULL;
    Linked_node *node_before = NULL;
    Linked_node *node_current = NULL;
    Linked_node *node_after = list->node;
    for (size_t i = 0; i < position + 1; i++) {
        node_before = node_current;
        node_current = node_after;
        node_after = node_after->next;
    }
    if (node_before != NULL) linked_node_set_next(node_before, node_after);
    if(data!=NULL) *data = node_current->data;

    if (node_after == NULL) {       // ultimo elemento
        if (node_before == NULL) {  // unico elemento
            free(list->node);
            linked_list_node_set(list, NULL);
        } else {  // nao e o unico elemento
            linked_node_set_next(node_before, NULL);
            free(node_current);
        }
    } else {                        // nao e o ultimo elemento
        if (node_before == NULL) {  // primeiro elemento
            Linked_node *node = list->node;
            linked_list_node_set(list, list->node->next);
            free(node);
        } else {  // nao e o primeiro elemento
            linked_node_set_next(node_before, node_after);
            linked_node_set_next(node_current, NULL);
            node_current->data = NULL;
            free(node_current);
        }
    }

    linked_list_size_edit(list, list->size - 1);
    return data;
}
bool linked_get_at(const Linked_head *list, size_t position, void **data) {
    if (list == NULL) {
        errno = EINVAL;
        return NULL;
    }
    if (position >= list->size) {
        errno = EINVAL;
        return NULL;
    }
    Linked_node *node = list->node;
    for (size_t i = 0; i < position; i++) {
        node = node->next;
    }
    *data = node->data;
    return true;
}

void linked_delete(Linked_head **list) {
    if (*list == NULL || list == NULL) {
        errno = EINVAL;
        return;
    }
    if ((*list)->node == NULL) {
        free(list);
        *list = NULL;
        return;
    }
    Linked_node *node = (*list)->node;
    Linked_node *nodeNext = (*list)->node->next;

    while (node) {
        nodeNext = node->next;
        free(node);
        node = nodeNext;
    }
    *list = NULL;
}

bool linked_get_node_at(const Linked_head *list, size_t position, Linked_node **node) {
    if (list == NULL) {
        errno = EINVAL;
        return false;
    }
    if (position >= list->size) {
        errno = EINVAL;
        return false;
    }
    *node = list->node;
    for (size_t i = 0; i < position; i++) *node = (*node)->next;
    return true;
}

Linked_head *linked_merge_sort(Linked_head **list_left,
                               Linked_head **list_right,
                               bool (*organizer)(const void *, const void *)) {
    Linked_head *list_result = NULL;
    linked_create(&list_result);
    Linked_node *left = (*list_left)->node;
    Linked_node *right = (*list_right)->node;
    while (left != NULL && right != NULL) {
        if (organizer(left->data, right->data)) {
            linked_append(list_result, left->data);
            left = left->next;
        } else {
            linked_append(list_result, right->data);
            right = right->next;
        }
    }
    while (left != NULL) {
        linked_append(list_result, left->data);
        left = left->next;
    }
    while (right != NULL) {
        linked_append(list_result, right->data);
        right = right->next;
    }
    linked_delete(list_left);
    linked_delete(list_right);
    return list_result;
}

Linked_head *linked_sort(Linked_head **list,
                         bool (*organizer)(const void *, const void *)) {
    if (list == NULL) {
        errno = EINVAL;
        return NULL;
    }
    if (organizer == NULL) {
        errno = EINVAL;
        return NULL;
    }
    *list = linked_sort_internal(*list, organizer);
    return *list;
}

Linked_head *linked_sort_internal(Linked_head *list,
                                  bool (*organizer)(const void *, const void *)) {
    // Se a lista estiver vazia ou tiver apenas um elemento, ela já está
    // ordenada
    if (list->node == NULL || list->size <= 1) {
        return list;
    }
    Linked_head *left_list = NULL;
    Linked_head *right_list = NULL;

    linked_create(&left_list);
    linked_create(&right_list);
    // Inicializa dois ponteiros, 'slow' e 'fast', para dividir a lista ao meio
    Linked_node *slow = list->node;
    Linked_node *fast = list->node;

    // Enquanto 'fast' não atingir o final da lista
    while (fast != NULL && fast->next != NULL) {
        linked_append(left_list, slow->data);
        // Move 'slow' uma posição por vez e 'fast' duas posições por vez
        slow = slow->next;
        fast = fast->next->next;
    }
    while (slow != NULL) {
        linked_append(right_list, slow->data);
        slow = slow->next;
    }
    linked_delete(&list);
    // Classifica recursivamente as duas metades da lista
    left_list = linked_sort_internal(left_list, organizer);
    right_list = linked_sort_internal(right_list, organizer);
    // Mescla as duas listas ordenadas usando a função 'merge'
    return linked_merge_sort(&left_list, &right_list, organizer);
}

long linked_search(const Linked_head *list, Linked_head **positions,
                   const void *search, bool (*searchFunc)(const void *, const void *)) {
    if (list == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (search == NULL) {
        errno = EINVAL;
        return -1;
    }
    Linked_node *node_current = list->node;
    if (positions != NULL) linked_create(positions);
    long position_current = -1;
    while (node_current != NULL) {
        position_current++;
        if (searchFunc(search, node_current->data)) {
            if (positions != NULL)
                linked_push(*positions, node_current->data);
            else
                return position_current;
        }
        node_current = node_current->next;  // Avança para o próximo nó da lista
    }
    return -1;
}

Linked_head *linked_merge(const Linked_head *list1, const Linked_head *list2) {
    if (list1 == NULL || list2 == NULL) {
        errno = EINVAL;
        return NULL;
    }
    Linked_head *list_new;
    linked_create(&list_new);
    Linked_node *crawler = list1->node;
    while (crawler != NULL) {
        linked_append(list_new, crawler->data);
        crawler = crawler->next;
    }
    crawler = list2->node;
    while (crawler != NULL) {
        linked_append(list_new, crawler->data);
        crawler = crawler->next;
    }
    return list_new;
}

inline Linked_node *linked_node_create(const void *data) {
    Linked_node *node = (Linked_node *)malloc(sizeof(Linked_node));
    if (node == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    node->data = (void *)data;
    linked_node_set_next(node, NULL);
    return node;
}

inline void linked_node_set_next(const Linked_node *node,
                                 Linked_node *next) {
    *(Linked_node **)(&node->next) = next;
}

inline void linked_list_size_edit(const Linked_head *list, size_t value) {
    *(size_t *)(&list->size) = value;
}
inline void linked_list_node_set(const Linked_head *list, Linked_node *node) {
    *(Linked_node **)(&list->node) = node;
}