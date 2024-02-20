#include "linked_list.h"

#include <stdint.h>
inline Linked_node *linked_node_create(void *data);

Linked_head *linked_create(void *data, bool initialized) {
    Linked_head *head = (Linked_head *)malloc(sizeof(Linked_head));
    if (!initialized || data != NULL) {
        head->node = NULL;
        head->size = 0;
    } else {
        head->node = linked_node_create(data);
        head->size = 1;
    }
    return head;
}
Linked_head *linked_push(Linked_head *list, void *data) {
    Linked_node *node = linked_node_create(data);
    if (!list->size) {
        list->node = node;
    } else {
		linked_node_set_next(node->next,list->node);
        list->node = node;
    }
    list->size++;
    return list;
}

Linked_head *linked_add_at(Linked_head *list, void *data, size_t position) {
    if (!position) return linked_push(list, data);

    Linked_node *node_current = list->node;
    Linked_node *node_next = list->node;
    for (size_t i = 0; i < position; i++) {
        node_current = node_next;
        node_next = node_next->next;
    }
    Linked_node *node = linked_node_create(data);
    node_current->next = node;
    node->next = node_next;
    return list;
}

void *linked_pop(Linked_head *list) {
    if (!list->size) return NULL;
    Linked_node *node_last = list->node;
    Linked_node *node_penult = list->node;
    void *data = NULL;
    while (node_last->next != NULL) {
        node_penult = node_last;
        node_last = node_last->next;
    }
    data = node_last->data;
    node_penult->next = NULL;
    free(node_last);
    return data;
}
void *linked_remove_at(Linked_head *list, size_t position) {
    if (!list->size || position >= list->size) return NULL;
    Linked_node *node_before = NULL;
    Linked_node *node_current = NULL;
    Linked_node *node_after = list->node;
    for (size_t i = 0; i < position + 1; i++) {
        node_before = node_current;
        node_current = node_after;
        node_after = node_after->next;
    }
    if (node_before != NULL) node_before->next = node_after;
    void *data = node_current->data;

    if (node_after == NULL) {       // ultimo elemento
        if (node_before == NULL) {  // unico elemento
            free(list->node) list->node = NULL;
        } else {  // nao e o unico elemento
            node_before->next = NULL;
            free(node_current);
        }
    } else {                        // nao e o ultimo elemento
        if (node_before == NULL) {  // primeiro elemento
            Linked_node *node = list->node;
            list->node = list->node->next;
            free(node);
        } else {  // nao e o primeiro elemento
            node_before->next = node_after;
            node_current->next = NULL;
            node_current->data = NULL;
            free(node_current);
        }
    }
    list->size--;
    return data;
}
void *linked_get_at(Linked_head *list, size_t position) {
    if (position >= list->size) return NULL;
    Linked_node *node = list->node;
    for (size_t i = 0; i < position; i++) {
        node = node->next;
    }
    return node->data;
}

void linked_delete(Linked_head **list) {
    if ((*list)->node == NULL) {
        free(list);
        *list = NULL;
        return;
    }
    Linked_node *node = (*list)->node;
    Linked_node *nodeNext = (*list)->node->next;
    do {
        free(node);
        node = nodeNext->next;
        nodeNext = node->next;
    } while (node);
    *list = NULL;
}

void merge(Linked_node **list, bool (*organizer)(void *, void *)) {
    Linked_node dummy;
    Linked_node *tail = &dummy;
    dummy.next = NULL;

    Linked_node *left = *list;
    Linked_node *right = (*list)->next;
    while (left && right) {
        if (organizer(left->data, right->data)) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }
    if (left) {
        tail->next = left;
    } else {
        tail->next = right;
    }

    (*list) = dummy.next;
}

void linked_sort(Linked_head **list, bool (*organizer)(void *, void *)) {
    // Se a lista estiver vazia ou tiver apenas um elemento, ela já está
    // ordenada
    if (list == NULL || !(*list)->next) {
        return;
    }

    // Inicializa dois ponteiros, 'slow' e 'fast', para dividir a lista ao meio
    Linked_node *slow = *list;
    Linked_node *fast = (*list)->next;

    // Enquanto 'fast' não atingir o final da lista
    while (fast && fast->next) {
        // Move 'slow' uma posição por vez e 'fast' duas posições por vez
        slow = slow->next;
        fast = fast->next->next;
    }

    // 'slow' agora aponta para o meio da lista, divide a lista em duas partes
    Linked_node *right = slow->next;
    slow->next = NULL;

    // Classifica recursivamente as duas metades da lista
    linked_sort(list, organizer);
    linked_sort(&right, organizer);

    // Mescla as duas listas ordenadas usando a função 'merge'
    merge(list, organizer);
}

bool linked_search(Linked_head *list, size_t *positions, size_t count,
                   void *search, bool (*searchFunc)(void *, void *)) {
    size_t position = 0;  // Variável para rastrear a posição atual na lista
    bool found = false;  // Variável para rastrear se pelo menos uma ocorrência
                         // foi encontrada
    size_t positionsPosition = 0;
    Linked_node *node_current = list->node;
    while (list) {
        if (searchFunc(search, list->node->data)) {
            if (found <
                count) {  // Verifique se o limite de posições foi atingido
                positions[positionsPosition++] =
                    position;  // Armazena a posição da ocorrência
                found = true;  // Define como true quando a primeira ocorrência
                               // é encontrada
            }
        }
        list = list->next;  // Avança para o próximo nó da lista
        position++;         // Incrementa a posição atual
    }

    return found;  // Retorna true se pelo menos uma ocorrência foi encontrada
}

inline Linked_node *linked_node_create(void *data) {
    Linked_node *node =
        (Linked_node *)malloc(sizeof(Linked_node));  // this is the node
    node->data = data;
    linked_node_set_next(node, NULL);
}

inline void linked_node_set_next(Linked_node *node, Linked_node *next) {
    *(Linked_node **)(&node->next) = next;
}
