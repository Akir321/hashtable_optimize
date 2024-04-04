#include <stdio.h>

#include "../include/hash_table.h"

const int NumberOfChains = 7;

int fakeHash(const char *key);



int main()
{
    HashTable *ht = hashTableCtor(NumberOfChains, fakeHash); 
    HASH_TABLE_DUMP(ht);

    val_t valBuf = 0;

    valBuf = 1;
    hashTableSet(ht, "abc", &valBuf);
    HASH_TABLE_DUMP(ht);

    valBuf = 2;
    hashTableSet(ht, "abc", &valBuf);
    HASH_TABLE_DUMP(ht);

    valBuf = 3;
    hashTableSet(ht, "ab", &valBuf);
    HASH_TABLE_DUMP(ht);

    valBuf = 4;
    hashTableSet(ht, "ba", &valBuf);
    HASH_TABLE_DUMP(ht);

    hashTableKeyDel(ht, "abc");
    HASH_TABLE_DUMP(ht);

    hashTableIncValByKey(ht, "ba");
    HASH_TABLE_DUMP(ht);

    hashTableAddKeyToMultiset(ht, "ccc");
    HASH_TABLE_DUMP(ht);

    hashTableDtor(&ht);

    printf("hash table ptr after destruction = %p\n", ht);
}


int fakeHash(const char *key)
{
    return key[0] % NumberOfChains;
}
