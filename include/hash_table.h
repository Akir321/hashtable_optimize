#ifndef  __HASH_TABLE_H__
#define  __HASH_TABLE_H__


#include "list.h"


const int StartListCapacity = 2;


struct HashTable
{
    int size;
    int chainsNum;    // should be a prime number

    int (*hashFunc)(const char *);

    List **chains;
};

enum HashTableErrors
{
    HT_KEY_NOT_PRESENT = -1,
};

#define HASH_TABLE_DUMP(ht) hashTableDump(ht, __FILE__,  __LINE__, __func__)


HashTable *hashTableCtor (int chainsNum, int (*hashFunc)(const char *));

int hashTableDtor             (HashTable **htPtr);

int hashTableDump             (HashTable *ht, 
                               const char *file, int line, const char *function);

int hashTableSet              (HashTable *ht, const char *key, val_t *valBuf);
int hashTableGet              (HashTable *ht, const char *key, val_t *valBuf);

int hashTableKeyDel           (HashTable *ht, const char *key);
int hashTableIncValByKey      (HashTable *ht, const char *key);

int hashTableAddKeyToMultiset (HashTable *ht, const char *key);


#endif //__HASH_TABLE_H__
