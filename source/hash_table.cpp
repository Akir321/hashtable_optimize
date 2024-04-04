#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/hash_table.h"
#include "../include/html_logfile.h"


#define ASSERT_HT { assert(ht);             \
                    assert(ht->chains);     \
                    assert(ht->hashFunc); }


static Data *hashTableGetElemPtr(HashTable *ht, const char *key, List **outListPtr);


HashTable *hashTableCtor(int chainsNum, int (*hashFunc)(const char *))
{
    HashTable *ht = (HashTable *)calloc(1, sizeof(HashTable));
    if (!ht) return NULL;

    ht->size      = 0;
    ht->chainsNum = chainsNum;

    ht->hashFunc  = hashFunc;

    ht->chains = (List **)calloc(ht->chainsNum, sizeof(List *));
    if (!ht->chains) { perror("ht->chains:"); free(ht); return NULL; }

    for (int i = 0; i < ht->chainsNum; i++)
    {
        ht->chains[i] = listCtor(StartListCapacity);
        if (!ht->chains[i])
        {
            printf("couldn't alloc ht->chains[%d]\n", i);
            return NULL;
        }
    }
    
    return ht;
}

int hashTableDtor(HashTable **htPtr)
{
    assert(htPtr);
    assert(*htPtr);
    assert((*htPtr)->chains);

    HashTable *ht = *htPtr;

    for (int i = 0; i < ht->chainsNum; i++)
    {
        listDtor(ht->chains + i);
    }

    free(ht);

    *htPtr = NULL;

    return EXIT_SUCCESS;
}

static Data *hashTableGetElemPtr(HashTable *ht, const char *key, List **outListPtr)
{
    ASSERT_HT;
    assert(key);

    int listIndex = ht->hashFunc(key);
    List *curList = ht->chains[listIndex];

    if (outListPtr) *outListPtr = curList;

    int elemIndex = listFindKey(curList, key);
    if (elemIndex == 0) return NULL;

    return listValuePtrByIndex(curList, elemIndex);
}

int hashTableSet(HashTable *ht, const char *key, val_t *valBuf)
{
    ASSERT_HT;
    assert(key);
    assert(valBuf);

    List *curList = NULL;
    Data *elemPtr = hashTableGetElemPtr(ht, key, &curList);

    Data data = {};
    dataAssign(&data, key, *valBuf);

    if   (!elemPtr) listPushFront(curList, &data);
    else            elemPtr->value = *valBuf;
    
    return EXIT_SUCCESS;
}

int hashTableGet(HashTable *ht, const char *key, val_t *valBuf)
{
    ASSERT_HT;
    assert(key);
    assert(valBuf);

    Data *elemPtr = hashTableGetElemPtr(ht, key, NULL);

    if (!elemPtr)   return HT_KEY_NOT_PRESENT;

    *valBuf = elemPtr->value;

    return EXIT_SUCCESS; 
}

int hashTableKeyDel(HashTable *ht, const char *key)
{
    ASSERT_HT;
    assert(key);

    int listIndex = ht->hashFunc(key);
    List *curList = ht->chains[listIndex];

    int elemIndex = listFindKey(curList, key);

    if (elemIndex == 0) return HT_KEY_NOT_PRESENT;

    listDel(curList, elemIndex);

    return EXIT_SUCCESS;
}

int hashTableIncValByKey(HashTable *ht, const char *key)
{
    ASSERT_HT;
    assert(key);

    Data *elemPtr = hashTableGetElemPtr(ht, key, NULL);val_t valBuf  = 1;

    if (!elemPtr) return HT_KEY_NOT_PRESENT;
    
    elemPtr->value++;

    return EXIT_SUCCESS;
}

int hashTableAddKeyToMultiset(HashTable *ht, const char *key)
{
    ASSERT_HT;
    assert(key);

    int   error  = hashTableIncValByKey(ht, key);
    val_t valBuf = 1;

    if (error == HT_KEY_NOT_PRESENT) hashTableSet(ht, key, &valBuf);

    return EXIT_SUCCESS;
}

