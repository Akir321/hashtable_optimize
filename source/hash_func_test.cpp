#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/hash_table.h"
#include "../include/hash_functions.h"

const int KeyStrLength = 256;

const int ChainsNum         = 8192;
const int ChainsNumChecksum = 599;

const char *FileNameIn = "words_changed.txt";

#define DATA_DIR  "hash_data/"
#define OUT_EXTEN ".csv"

int testHashFunc(int chainsNum, int (*hashFunc)(const char *), 
                 const char *fileNameIn, const char *fileNameOut);

HashTable *createHashTableWithFile(const char *fileNameIn, 
                                   int chainsNum, int (*hashFunc)(const char *));

int outputListSizes(HashTable *ht, const char *fileNameOut);

#define TEST_HASH_FUNC(chN, hashFunc) \
        testHashFunc  (chN, hashFunc, FileNameIn, DATA_DIR # hashFunc OUT_EXTEN)


int main()
{
    //TEST_HASH_FUNC(ChainsNum, hashAlways1);

    //TEST_HASH_FUNC(ChainsNum, hashFirstLetter);

    //TEST_HASH_FUNC(ChainsNum, hashChecksum);

    //TEST_HASH_FUNC(ChainsNumChecksum,   hashChecksum);

    //TEST_HASH_FUNC(ChainsNum, hashKeyLength);

    //TEST_HASH_FUNC(ChainsNum, hashRorXor);

    //TEST_HASH_FUNC(ChainsNum, hashRolXor);

    TEST_HASH_FUNC(ChainsNum, hashGNU);

    TEST_HASH_FUNC(ChainsNum, hashCRC32);

    TEST_HASH_FUNC(ChainsNum, hashCRC32Intrinsics);
}



int testHashFunc(int chainsNum, int (*hashFunc)(const char *), 
                 const char *fileNameIn, const char *fileNameOut)
{
    assert(hashFunc);
    assert(fileNameIn);
    assert(fileNameOut);

    HashTable *ht = createHashTableWithFile(fileNameIn, chainsNum, hashFunc);
    if (!ht) { printf("couldn't create hash table\n"); return EXIT_FAILURE; }

    outputListSizes(ht, fileNameOut);

    hashTableDtor(&ht);

    return EXIT_SUCCESS;
}

HashTable *createHashTableWithFile(const char *fileNameIn, 
                                   int chainsNum, int (*hashFunc)(const char *))
{
    assert(fileNameIn);
    assert(hashFunc);

    FILE *fileIn =  fopen(fileNameIn, "r");
    if  (!fileIn) { printf("couldn't open: %s\n", fileNameIn); return NULL; }

    HashTable *ht = hashTableCtor(chainsNum, hashFunc);

    char key[KeyStrLength] = "";

    int    amountRead  = fscanf(fileIn, "%s", key);
    while (amountRead != EOF)
    {
        hashTableAddKeyToMultiset(ht, key);

        amountRead = fscanf(fileIn, "%s", key);
    }
    fclose(fileIn);

    return ht;
}

int outputListSizes(HashTable *ht, const char *fileNameOut)
{
    assert(ht);
    assert(fileNameOut);

    FILE *fileOut  = fopen(fileNameOut, "w");
    if  (!fileOut) { printf("couldn't open: %s\n", fileNameOut); return EXIT_FAILURE; }

    for (int i = 0; i < ht->chainsNum; i++)
    {
        fprintf(fileOut, "%d,%d\n", i, ht->chains[i]->size);
    }

    fclose(fileOut);

    return EXIT_SUCCESS;
}
