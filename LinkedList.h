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
void listSort(LinkedNode *list, bool (*organizer)(void *, void *)) {
    size_t size = listSize(list, NULL);
    if (size <= 1)
        return;
    for (
        struct{LinkedNode *current;LinkedNode *before;}search = {list,NULL};
        search.current->next!=NULL; search.before = search.current, search.current = search.current->next) {
        for (
            struct {LinkedNode *current; LinkedNode *before;} crawler = {search.current->next, search.current};
            crawler.current != NULL; crawler.before = crawler.current, crawler.current = crawler.current->next) {
            if (!organizer(search.current->data, crawler.current->data))
                continue;
            LinkedNode *tmp = crawler.current->next;
            crawler.current->next = search.current->next;
            search.current->next = tmp;
            crawler.before = search.current;
            if (search.before != NULL) {
                search.before->next = crawler.current;
            }
        }
    }
}
bool listSearch(LinkedNode *list, size_t *result, void *search, bool (*searchFunc)(void *, void *)) {
    size_t size = listSize(list, NULL);
    for (size_t i = 0; i < size; i++) {
        if (searchFunc(search, list->data)) {
            if (result)
                *result = i;
            return true;
        }
        list = list->next;
    }
    return false;
}
bool listIsNodeValid(LinkedNode *node) {
    return node->isUsed;
}
#endif