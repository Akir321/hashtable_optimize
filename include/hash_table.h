#ifndef  __HASH_TABLE_H__
#define  __HASH_TABLE_H__


#include "list.h"


const int StartListCapacity = 8;


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


HashTable *hashTableCtor (int chainsNum, int (*hashFunc)(const char *));

int hashTableDtor             (HashTable **htPtr);

int hashTableSet              (HashTable *ht, const char *key, val_t *valBuf);
int hashTableGet              (HashTable *ht, const char *key, val_t *valBuf);

int hashTableKeyDel           (HashTable *ht, const char *key);
int hashTableIncValByKey      (HashTable *ht, const char *key);

int hashTableAddKeyToMultiset (HashTable *ht, const char *key);


#endif //__HASH_TABLE_H__
