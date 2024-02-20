#include "linked_list.h"

inline Linked_node* linked_node_create(void *data);

Linked_head *linked_create(void *data, bool initialized) {
    Linked_head *head = (Linked_head *)malloc(sizeof(Linked_head));
    if(!initialized || data != NULL){
        head->node = NULL;
        head->size = 0;
    }else{
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
        node->next = list->node;
        list->node = node;
    }
    list->size++;
    return list;
}

Linked_head *linked_add_at(Linked_head *list, void *data, size_t position) {
    if (!position) return linked_push(list, data);

    Linked_node *nodeCurrent = list->node;
    Linked_node *nodeNext = list->node;
    for (size_t i = 0; i < position; i++) {
        nodeCurrent = nodeNext;
        nodeNext = nodeNext->next;
    }
    Linked_node *node = linked_node_create(data);
    nodeCurrent->next = node;
    node->next = nodeNext;
    return list;
}

void *linked_pop(Linked_head *list) {
    if (!list->size) return NULL;
    Linked_node *nodeLast = list->node;
    Linked_node *nodePenult = list->node;
    void *data = NULL;
    while (nodeLast->next != NULL) {
        nodePenult = nodeLast;
        nodeLast = nodeLast->next;
    }
    data = nodeLast->data;
    nodePenult->next = NULL;
    if (nodeLast == nodePenult)
        nodeLast->isUsed = false;
    else
        free(nodeLast);
    return data;
}
void *linked_remove_at(Linked_head *list, size_t position) {
    if (!list->isUsed) return NULL;
    Linked_node *nodeBefore = NULL;
    Linked_node *nodeCurrent = NULL;
    Linked_node *nodeAfter = list;
    for (size_t i = 0; i < position + 1; i++) {
        nodeBefore = nodeCurrent;
        nodeCurrent = nodeAfter;
        nodeAfter = nodeAfter->next;
    }
    if (nodeBefore != NULL) nodeBefore->next = nodeAfter;
    void *data = nodeCurrent->data;

    if (nodeAfter == NULL) {       // ultimo elemento
        if (nodeBefore == NULL) {  // unico elemento
            nodeCurrent->data = NULL;
            nodeCurrent->isUsed = false;
        } else {  // nao e o unico elemento
            nodeBefore->next = NULL;
            free(nodeCurrent);
        }
    } else {                       // nao e o ultimo elemento
        if (nodeBefore == NULL) {  // primeiro elemento
            size_t size = linked_size(list, NULL);
            Linked_node *crawlerAfter = list->next;
            Linked_node *crawler = list;
            for (size_t i = 0; i < size; i++) {
                crawler->data = crawlerAfter->data;
                crawler->next = crawlerAfter->next;
                crawler = crawlerAfter;
                crawlerAfter = crawlerAfter->next;
            }
            crawler->next = NULL;
            free(crawlerAfter);
        } else {  // nao e o primeiro elemento
            nodeBefore->next = nodeAfter;
            nodeCurrent->next = NULL;
            nodeCurrent->data = NULL;
            free(nodeCurrent);
        }
    }
    return data;
}
void *linked_get_at(Linked_head *list, size_t position) {
    Linked_node *node = list;
    for (size_t i = 0; i < position; i++) {
        node = node->next;
    }
    return node->data;
}
size_t linked_size(Linked_head *list, bool *isCircular) {
    bool isCircularTemp = false;
    bool *isCirculatTempPtr = &isCircularTemp;
    bool **isCircularPtr = &isCirculatTempPtr;
    if (isCircular) isCircularPtr = &isCircular;
    if (!list->isUsed) {
        (**isCircularPtr) = false;
        return 0;
    }

    size_t size = 1;
    Linked_node *nodeStart = list;
    Linked_node *node = nodeStart;
    while (node != NULL && node->next != NULL && node->next != nodeStart) {
        size++;
        node = node->next;
    }
    (**isCircularPtr) = node->next != NULL;
    return size;
}
void linked_delete(Linked_head **list) {
    if (!(*list)->next) {
        free(list);
        list = NULL;
        return;
    }
    Linked_node *node = *list;
    Linked_node *nodeNext = (*list)->next;
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

void linked_sort(Linked_node **list, bool (*organizer)(void *, void *)) {
    // Se a lista estiver vazia ou tiver apenas um elemento, ela já está
    // ordenada
    if (!list || !(*list)->next) {
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

bool linked_search(Linked_node *list, size_t *positions, size_t count,
                 void *search, bool (*searchFunc)(void *, void *)) {
    size_t position = 0;  // Variável para rastrear a posição atual na lista
    bool found = false;  // Variável para rastrear se pelo menos uma ocorrência
                         // foi encontrada
    size_t positionsPosition = 0;
    while (list) {
        if (searchFunc(search, list->data)) {
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

inline bool linked_is_node_valid(Linked_node *node) { return node->isUsed; }

bool linked_is_node_valid(Linked_node *node) { return node->isUsed; }

inline Linked_node* linked_node_create(void *data){
    Linked_node *node = (Linked_node*)malloc(sizeof(Linked_node));
    node->data = data;
    node->next = NULL;
}