#ifndef  __HASH_FUNCTIONS_H__
#define  __HASH_FUNCTIONS_H__

#include <stdint.h>
#include <nmmintrin.h>

#ifndef  HASH_MOD
#define  HASH_MOD 9973
#endif //HASH_MOD


int hashAlways1         (const char *key);
int hashFirstLetter     (const char *key);
int hashChecksum        (const char *key);
int hashKeyLength       (const char *key);
int hashRorXor          (const char *key);
int hashRolXor          (const char *key);
int hashGNU             (const char *key);
int hashCRC32           (const char *key);
int hashCRC32Intrinsics (const char *key);

uint32_t ror_32t(uint32_t num);
uint32_t rol_32t(uint32_t num);


int hashAlways1(const char *key)
{
    return 1;
}

int hashFirstLetter(const char *key)
{
    return key[0];
}

int hashChecksum(const char *key)
{
    int  checksum = 0;
    int i         = 0;

    while(key[i] != '\0') { checksum += key[i++]; }

    return checksum;
}

int hashKeyLength(const char *key)
{
    int length = 0;
    while(key[length++] != '\0') {}

    return length;
}

int hashRorXor(const char *key)
{
    int hash = 0;

    while (*key != '\0')
    {
        hash = ror_32t(hash) ^ *key++;
    }

    return hash;
}

int hashRolXor(const char *key)
{
    int hash = 0;

    while (*key != '\0')
    {
        hash = rol_32t(hash) ^ *key++;
    }

    return hash;
}

int hashGNU(const char *key)
{
    int hash = 0;

    while(*key != '\0')
    {
        hash = (hash << 5) - hash; // hash = hash * 31;
        hash = hash + *key++;
        hash = hash % HASH_MOD;
    }

    return hash;
}

uint32_t ror_32t(uint32_t num)
{
    return (num >> 1) + (num << (sizeof(num) * 8 - 1));
}

uint32_t rol_32t(uint32_t num)
{
    return (num << 1) + (num >> (sizeof(num) * 8 - 1));
}

int hashCRC32(const char *key)
{
    unsigned crc  = 0xFFFFFFFF;
    unsigned mask = 0;

    while (*key)
    {
        crc = crc ^ (unsigned)(*key++);

        for (int i = 7; i >= 0; i--)
        {
            mask = -(crc &  1);
            crc  =  (crc >> 1) ^ (0x04C11DB7 & mask);
        }
    }

    return crc;
}

int hashCRC32Intrinsics(const char *key)
{
    unsigned hash = 0;

    hash = (unsigned) _mm_crc32_u64(hash, *((const uint64_t*)key + 0));
    hash = (unsigned) _mm_crc32_u64(hash, *((const uint64_t*)key + 1));
    hash = (unsigned) _mm_crc32_u64(hash, *((const uint64_t*)key + 2));
    hash = (unsigned) _mm_crc32_u64(hash, *((const uint64_t*)key + 3));

    return hash;
}


#undef HASH_MOD

#endif //__HASH_FUNCTIONS_H__
