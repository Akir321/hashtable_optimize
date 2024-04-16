#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <nmmintrin.h>

#include "../include/hash_table.h"

const int KeyStrLength = 32;

const int WordsAmount  = 63994;
const int ChainsNum    = 8192;

const int NumOfRuns    = 5000;
 
const char *FileNameIn = "words_changed.txt";

val_t Number = 0;


static inline int hashGNU(const char *key);

static inline int hashCRC32Intrinsics(const char *key);

static inline uint64_t getProcessorTime();

HashTable *createHashTableWithFile(const char *fileNameIn, 
                                   int chainsNum, int (*hashFunc)(const char *));

static char *myStrdupLen32(const char *key);

char **readArrFromFile(const char *fileNameIn, int wordsCount);
int destrStrArr(char **strArr, int wordsCount);


int main()
{
    HashTable *ht = createHashTableWithFile(FileNameIn, ChainsNum, hashCRC32Intrinsics);

    char **words = readArrFromFile(FileNameIn, WordsAmount);

    //printf("before getProcTime()\n");
    uint64_t begin = getProcessorTime();
    //printf("begin = %ld\n", begin);

    for (int runsCnt = 0; runsCnt < NumOfRuns; runsCnt++)
    {
        for (size_t wordIndex = 0; wordIndex < WordsAmount; wordIndex++)
        {
            hashTableGet(ht, words[wordIndex], &Number);
        }

        if (runsCnt % 500 == 0) printf("performed %d runs\n", runsCnt);
    }

    uint64_t result = getProcessorTime() - begin;

    printf("Measurement is in processor ticks\n");
    printf("cycles performed: %d\n", NumOfRuns);
    printf("the result:       %ld\n", result);
    printf("time per cycle:   %ld\n", result / NumOfRuns);

    destrStrArr(words, WordsAmount);
}


HashTable *createHashTableWithFile(const char *fileNameIn, 
                                   int chainsNum, int (*hashFunc)(const char *))
{
    assert(fileNameIn);
    assert(hashFunc);

    FILE *fileIn =  fopen(fileNameIn, "r");
    if  (!fileIn) { printf("couldn't open: %s\n", fileNameIn); return NULL; }

    HashTable *ht = hashTableCtor(chainsNum, hashFunc);

    char key[KeyStrLength + 1] = "";

    int    amountRead  = fscanf(fileIn, "%s", key);
    while (amountRead != EOF)
    {
        hashTableAddKeyToMultiset(ht, key);

        memset(key, '\0', KeyStrLength + 1);
        amountRead = fscanf(fileIn, "%s", key);
    }
    fclose(fileIn);

    return ht;
}

char **readArrFromFile(const char *fileNameIn, int wordsCount)
{
    assert(fileNameIn);

    FILE *fileIn =  fopen(fileNameIn, "r");
    if  (!fileIn) { printf("couldn't open: %s\n", fileNameIn); return NULL; }

    char word[KeyStrLength + 1] = "";

    char **strArr = (char **)calloc(wordsCount, sizeof(char *));
    if (!strArr) return NULL;

    for (int i = 0; i < wordsCount; i++)
    {
        if (!fscanf(fileIn, "%s", word)) return NULL;
        strArr[i] = myStrdupLen32(word);
    }

    return strArr;
}

int destrStrArr(char **strArr, int wordsCount)
{
    assert(strArr);

    for (int i = 0; i < wordsCount; i++)
    {
        free(strArr[i]);
    }

    free(strArr);

    return EXIT_SUCCESS;
}


static inline uint64_t getProcessorTime()
{
    uint64_t time = 0;
     
    asm volatile 
    (
        ".intel_syntax noprefix\n\t"
        "xor rax, rax\n\t"
        "rdtsc\n\t"
        "shl rdx, 32\n\t"
        "add rax, rdx\n\t"
        "movq %0, rax\n\t"
        ".att_syntax prefix"
        : "=r"(time)
        :
        : "%rdx"
    );

    return time;
}

/*
int hashGNU(const char *key)
{
    int hash = 0;

    while(*key != '\0')
    {
        hash = (hash << 5) - hash; // hash = hash * 31;
        hash = hash + *key++;
        hash = hash % ChainsNum;
    }

    return hash;
}*/

static inline int hashGNU(const char *key)
{
    unsigned hash = 0;

    do 
    {
        hash = (hash << 5) - hash;
        hash = hash + *key;
        hash = hash % ChainsNum;
    }   while (*key++ != '\0');

    return hash;
}

static inline int hashCRC32Intrinsics(const char *key)
{
    unsigned hash = 0;

    hash = (unsigned) _mm_crc32_u64(hash, *((const uint64_t *)key + 0));
    hash = (unsigned) _mm_crc32_u64(hash, *((const uint64_t *)key + 1));
    hash = (unsigned) _mm_crc32_u64(hash, *((const uint64_t *)key + 2));
    hash = (unsigned) _mm_crc32_u64(hash, *((const uint64_t *)key + 3));

    return hash;
}

static char *myStrdupLen32(const char *key)
{
    assert(key);

    char *dupStr = (char *)calloc(32 + 1, sizeof(char));
    if (!dupStr) return NULL;

    strncpy(dupStr, key, 32);
    dupStr[32] = '\0';

    return dupStr;
}

