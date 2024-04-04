#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/html_logfile.h"
#include "../include/list.h"


const Data Poison = {NULL, -1, -1};


#define LIST_VERIFY            \
    if (listError(list))       \
    {                          \
        LIST_DUMP(list);       \
        return listError(list);\
    }


void dataFprintf(const Data *data, FILE *file)
{
    assert(data);

    fprintf(file, "<%s> %d %d", data->str, data->strLength, data->value);
}

void dataAssign(Data *data, const char *key, val_t val)
{
    assert(data);

    LOG("key = %s\n", key);

    data->strLength = (int)   strlen(key);
    data->str       =         strdup(key);

    data->value     = val;
}

void dataDtor(Data *data)
{
    assert(data);

    free(data->str);
    data->strLength = -1;
    data->value     = -1;
}

void dataCopy(Data *dst, const Data *src)
{
    assert(dst);
    assert(src);

    memmove(dst, src, sizeof(Data));
}

int listFindKey(List *list, const char *key)
{
    assert(list);
    assert(key);

    int current = list->nodes[0].next;
    while (current > 0)
    {
        if (strcmp(list->nodes[current].data.str, key) == 0)
          { return current; }

        current = listNextIndex(list, current);
    }

    return 0;
}

List *listCtor(int capacity)
{
    List *list = (List *)calloc(1, sizeof(List));
    if (!list) return NULL;

    list->freeHead = 1;

    list->capacity = capacity;
    list->size     = 0;

    list->nodes = (ListNode *)calloc(capacity + 1, sizeof(ListNode));
    if (!list->nodes) { perror("data->next:"); return NULL; }


    dataCopy(&list->nodes[0].data, &Poison);
    list->nodes[0].next = 0;      //head
    list->nodes[0].prev = 0;      //tail

    for (int i = 1; i <= capacity; i++)
    {
        list->nodes[i].next = - i - 1;  // link the list of free nodes
        list->nodes[i].prev =     - 1;
    }

    return list;
}

int listDtor(List **listPtr)
{
    assert(listPtr);
    assert(*listPtr);

    List *list = *listPtr;

    list->freeHead = -1;

    list->capacity = 0;
    list->size     = -1;

    for (int i = 1; i <= list->capacity; i++)
    {
        dataDtor(&list->nodes[i].data);
    }

    free(list->nodes);
    list->nodes = NULL;

    free(list);
    *listPtr = NULL;

    return EXIT_SUCCESS;
}

int listError(List *list)
{
    assert(list);

    if (!list->nodes) return LIST_NODES_NULL;

    if (list->capacity < list->size)          return LIST_OVERFLOW;

    if (list->nodes[0].next > list->capacity) return LIST_BAD_VARIABLES;
    if (list->nodes[0].prev > list->capacity) return LIST_BAD_VARIABLES;
    //if (list->free  > list->capacity) return LIST_BAD_VARIABLES;

    int current = list->nodes[0].next;
    while (listNextIndex(list, current) > 0)
    {
        int nextElem = listNextIndex(list, current);
        if (current != listPrevIndex(list, nextElem)) return LIST_BAD_POINTERS;

        current = nextElem;
    }

    if (current != listTailIndex(list)) return LIST_BAD_POINTERS;

    return EXIT_SUCCESS;
}

int listDump(List *list, const char *file, int line, const char *function)
{
    assert(list);

    LOG("I'm listDump() called from %s %s(%d)\n", function, file, line);

    LOG(" capacity = %d\n", list->capacity);
    LOG(" size     = %d\n", list->size);

    LOG("  head    = %d\n", list->nodes[0].next);
    LOG("  tail    = %d\n", list->nodes[0].prev);
    LOG("  free    = %d\n", list->freeHead);

    LOG("Arrays:\n");
    LOG("index  data  next  prev:\n");

    for (int i = 0; i <= list->capacity; i++)
    {
        if      (i == 0)                   LOG("-")
        else if (list->nodes[i].prev >= 0) LOG("*")
        else                               LOG(" ")

        LOG("[%2d] ",i);
        dataFprintf(&list->nodes[i].data, LogFile);
        LOG(" %5d %5d\n", list->nodes[i].next, list->nodes[i].prev);
    }
    LOG("\n\n");

    return EXIT_SUCCESS;
}

int listAddAfter(List *list, int arrayAnchorIndex, elem_t *value)
{
    LIST_VERIFY;

    if (list->size >= list->capacity)
    {
        int error = listReallocUp(list, ListReallocRate);
        if (error) return error;
    }
    LIST_VERIFY;

    if (arrayAnchorIndex > list->capacity)      return LIST_INVALID_INDEX;
    if (list->nodes[arrayAnchorIndex].prev < 0) return LIST_INVALID_INDEX;

    int index      =  list->freeHead;
    list->freeHead = -list->nodes[index].next;

    dataCopy(&list->nodes[index].data, value);

    list->nodes[index].next            = list->nodes[arrayAnchorIndex].next;
    int nextIndex                      = list->nodes[index].next;
    list->nodes[nextIndex].prev        = index; 

    list->nodes[index].prev            = arrayAnchorIndex;
    list->nodes[arrayAnchorIndex].next = index;

    list->size++;

    LIST_VERIFY;
    return index;
}

int listAddBefore(List *list, int arrayAnchorIndex, elem_t *value)
{
    return listAddAfter(list, listPrevIndex(list, arrayAnchorIndex), value);
}

int listPushFront(List *list, elem_t *value)
{
    return listAddAfter(list, 0, value);
}

int listPushBack(List *list, elem_t *value)
{
    return listAddBefore(list, 0, value);
}

int listDel(List *list, int arrayElemIndex)
{
    LIST_VERIFY;

    if (arrayElemIndex <= 0)                    return LIST_INVALID_INDEX;
    if (arrayElemIndex >= list->capacity)       return LIST_INVALID_INDEX;
    if (list->nodes[arrayElemIndex].prev == -1) return LIST_INVALID_INDEX;

    int indexPrev = list->nodes[arrayElemIndex].prev;
    int indexNext = list->nodes[arrayElemIndex].next;

    list->nodes[indexPrev].next = indexNext;
    list->nodes[indexNext].prev = indexPrev;

    dataCopy(&list->nodes[arrayElemIndex].data, &Poison);
    list->nodes[arrayElemIndex].prev = -1;
    list->nodes[arrayElemIndex].next = -list->freeHead;
    list->freeHead                   = arrayElemIndex;

    list->size--;

    LIST_VERIFY;
    return EXIT_SUCCESS;
}

int listReallocUp(List *list, int reallocRate)
{
    LIST_VERIFY;

    if (reallocRate <= 0) return LIST_BAD_CAPACITY;

    int prevCapacity = list->capacity;
    list->capacity  *= reallocRate;


    list->nodes = (ListNode *)realloc( list->nodes, 
                                      (list->capacity + 1) * sizeof(ListNode));

    if (!list->nodes) { perror("list->nodes:"); return LIST_MEMORY_ERROR; }

    for (int i = prevCapacity + 1; i <= list->capacity; i++)
    {
        dataCopy(&list->nodes[i].data, &Poison);
        list->nodes[i].next = - i - 1;
        list->nodes[i].prev =     - 1;
    }

    LIST_VERIFY;
    return EXIT_SUCCESS;
}
