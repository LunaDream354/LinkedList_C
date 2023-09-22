#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdbool.h>
#include <stdlib.h>
typedef struct LinkedNode {
    void *data;
    bool isUsed;
    struct LinkedNode *next;
} LinkedNode;

LinkedNode *listCreate(void *data, bool initialized);
LinkedNode *listPush(LinkedNode *list, void *data);
LinkedNode *listAddAt(LinkedNode *list, void *data, size_t position);
void *listPop(LinkedNode *list);
void *listRemoveAt(LinkedNode *list, size_t position);
// void** listRemoveRange(LinkedNode *list, size_t start,size_t end);
void *listGetAt(LinkedNode *list, size_t position);
LinkedNode *listGetNodeAt(LinkedNode *list, size_t position);
size_t listSize(LinkedNode *list, bool *isCIrcular);
void listDelete(LinkedNode **list);
void listSort(LinkedNode *list, bool (*func)(void *, void *));
bool listSearch(LinkedNode *list, size_t *result, void *search, bool (*searchFunc)(void *, void *));
bool listIsNodeValid(LinkedNode *node);

LinkedNode *listCreate(void *data, bool initialized) {
    LinkedNode *head = (LinkedNode *)malloc(sizeof(LinkedNode));
    head->data = data;
    head->isUsed = initialized;
    head->next = NULL;
    return head;
}
LinkedNode *listPush(LinkedNode *list, void *data) {
    if (list->isUsed) {
        LinkedNode *node = listCreate(list->data, true);
        list->data = data;
        list->next = node;
    } else {
        list->isUsed = true;
        list->data = data;
        list->next = NULL;
    }
    return list;
}

LinkedNode *listAddAt(LinkedNode *list, void *data, size_t position) {
    if (!position)
        return listPush(list, data);

    LinkedNode *nodeCurrent = list;
    LinkedNode *nodeNext = list;
    for (size_t i = 0; i < position; i++) {
        nodeCurrent = nodeNext;
        nodeNext = nodeNext->next;
    }
    LinkedNode *node = listCreate(data, true);
    nodeCurrent->next = node;
    node->next = nodeNext;
    return list;
}

void *listPop(LinkedNode *list) {
    if (list->isUsed == false)
        return NULL;
    LinkedNode *nodeLast = list;
    LinkedNode *nodePenult = list;
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
void *listRemoveAt(LinkedNode *list, size_t position) {
    if (!list->isUsed)
        return NULL;
    LinkedNode *nodeBefore = NULL;
    LinkedNode *nodeCurrent = NULL;
    LinkedNode *nodeAfter = list;
    for (size_t i = 0; i < position + 1; i++) {
        nodeBefore = nodeCurrent;
        nodeCurrent = nodeAfter;
        nodeAfter = nodeAfter->next;
    }
    if (nodeBefore != NULL)
        nodeBefore->next = nodeAfter;
    void *data = nodeCurrent->data;

    if (nodeAfter == NULL) {      // ultimo elemento
        if (nodeBefore == NULL) { // unico elemento
            nodeCurrent->data = NULL;
            nodeCurrent->isUsed = false;
        } else { // nao e o unico elemento
            nodeBefore->next = NULL;
            free(nodeCurrent);
        }
    } else {                      // nao e o ultimo elemento
        if (nodeBefore == NULL) { // primeiro elemento
            size_t size = listSize(list, NULL);
            LinkedNode *crawlerAfter = list->next;
            LinkedNode *crawler = list;
            for (size_t i = 0; i < size; i++) {
                crawler->data = crawlerAfter->data;
                crawler->next = crawlerAfter->next;
                crawler = crawlerAfter;
                crawlerAfter = crawlerAfter->next;
            }
            crawler->next = NULL;
            free(crawlerAfter);
        } else { // nao e o primeiro elemento
            nodeBefore->next = nodeAfter;
            nodeCurrent->next = NULL;
            nodeCurrent->data = NULL;
            free(nodeCurrent);
        }
    }
    return data;
}
/*
void **listRemoveRange(LinkedNode *list, size_t start, size_t end) {
    size_t size = end - start + 1;
    void **listOut = (void **)malloc(sizeof(void *) * (size));
    LinkedNode *nodeStart = list;
    for (size_t i = 0; i < start - 1; i++)
        nodeStart = nodeStart->next;
    LinkedNode *nodeCurrent = nodeStart;
    for (size_t i = 0; i <size;i++){
        listOut[i] = nodeCurrent->data;
        nodeCurrent = nodeCurrent->next;
    }
}
*/
void *listGetAt(LinkedNode *list, size_t position) {
    LinkedNode *node = list;
    for (size_t i = 0; i < position; i++) {
        node = node->next;
    }
    return node->data;
}
LinkedNode *listGetNodeAt(LinkedNode *list, size_t position) {
    for (size_t i = 0; i < position; i++) {
        list = list->next;
    }
    return list;
}

size_t listSize(LinkedNode *list, bool *isCircular) {
    bool isCircularTemp = false;
    bool *isCirculatTempPtr = &isCircularTemp;
    bool **isCircularPtr = &isCirculatTempPtr;
    if (isCircular)
        isCircularPtr = &isCircular;
    if (!list->isUsed) {
        (**isCircularPtr) = false;
        return 0;
    }

    size_t size = 1;
    LinkedNode *nodeStart = list;
    LinkedNode *node = nodeStart;
    while (node != NULL && node->next != NULL && node->next != nodeStart) {
        size++;
        node = node->next;
    }
    (**isCircularPtr) = node->next != NULL;
    return size;
}
void listDelete(LinkedNode **list) {
    if (!(*list)->next) {
        free(list);
        list = NULL;
        return;
    }
    LinkedNode *node = *list;
    LinkedNode *nodeNext = (*list)->next;
    do {
        free(node);
        node = nodeNext->next;
        nodeNext = node->next;
    } while (node);
    *list = NULL;
}

void merge(LinkedNode **list, bool(*organizer)(void *,  void *)) {
    LinkedNode dummy;
    LinkedNode *tail = &dummy;
    dummy.next = NULL;

    LinkedNode *left = *list;
    LinkedNode *right = (*list)->next;
    // Enquanto ambas as listas 'left' e 'right' não estiverem vazias
    while (left && right) {
        // Compara os elementos em 'left' e 'right' usando a função de comparação 'organizer'
        if (organizer(left->data, right->data)) {
            // Se o elemento em 'left' for menor ou igual ao elemento em 'right',
            // adiciona 'left' à nova lista mesclada e avança 'left'
            tail->next = left;
            left = left->next;
        }
        else {
            // Caso contrário, adiciona 'right' à nova lista mesclada e avança 'right'
            tail->next = right;
            right = right->next;
        }
        // Move o ponteiro 'tail' para o último nó adicionado à nova lista mesclada
        tail = tail->next;
    }
    // Após o loop, uma das listas 'left' ou 'right' ainda pode conter elementos não processados
    if (left) {
        tail->next = left;
    }
    else {
        // Adiciona todos os elementos restantes de 'right' à nova lista mesclada
        tail->next = right;
    }

    (*list) = dummy.next;
}


void listSort(LinkedNode **list, bool (*organizer)(void *, void *)) {
    // Se a lista estiver vazia ou tiver apenas um elemento, ela já está ordenada
    if (!list || !(*list)->next) {
        return;
    }

    // Inicializa dois ponteiros, 'slow' e 'fast', para dividir a lista ao meio
    LinkedNode *slow = *list;
    LinkedNode *fast = (*list)->next;

    // Enquanto 'fast' não atingir o final da lista
    while (fast && fast->next) {
        // Move 'slow' uma posição por vez e 'fast' duas posições por vez
        slow = slow->next;
        fast = fast->next->next;
    }

    // 'slow' agora aponta para o meio da lista, divide a lista em duas partes
    LinkedNode *right = slow->next;
    slow->next = NULL;

    // Classifica recursivamente as duas metades da lista
    listSort(list, organizer);
    listSort(&right, organizer);

    // Mescla as duas listas ordenadas usando a função 'merge'
    merge(list, organizer);
}

bool listSearch(LinkedNode *list, void *search, bool (*searchFunc)(void *, void *)) {
    size_t position = 0; // Variável para rastrear a posição atual na lista
    bool found = false; // Variável para rastrear se pelo menos uma ocorrência foi encontrada

    while (list) {
        if (searchFunc(search, list->data)) {
            found = true; // Define como true quando uma ocorrência é encontrada
            printf("Elemento encontrado na posição %zu: ", position); // Imprime a posição da ocorrência
            printf("%p\n", list->data); // Imprime o elemento encontrado (assumindo que o elemento é um ponteiro)
        }
        list = list->next; // Avança para o próximo nó da lista
        position++; // Incrementa a posição atual
    }

    if (!found) {
        printf("Elemento não encontrado na lista.\n"); // Imprime se nenhuma ocorrência for encontrada
    }

    return found; // Retorna true se pelo menos uma ocorrência foi encontrada
}

inline bool listIsNodeValid(LinkedNode *node) {
    return node->isUsed;
}

bool listIsNodeValid(LinkedNode *node) {
    return node->isUsed;
}
#endif