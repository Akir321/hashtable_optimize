#ifndef  __HASH_FUNCTIONS_H__
#define  __HASH_FUNCTIONS_H__

#ifndef  HASH_MOD
#define  HASH_MOD 599
#endif //HASH_MOD


int hashAlways1     (const char *key);
int hashFirstLetter (const char *key);
int hashChecksum    (const char *key);
int hashKeyLength   (const char *key);
int hashRorXor      (const char *key);
int hashRolXor      (const char *key);
int hashGNU         (const char *key);

int ror_32t(int num);
int rol_32t(int num);


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
        hash = (hash << 5) + hash; // hash = hash * 33;
        hash = hash + *key++;
        hash = hash % HASH_MOD;
    }

    return hash;
}

int ror_32t(int num)
{
    return (num << 1) + (num >> (sizeof(num) - 1));
}

int rol_32t(int num)
{
    return (num >> 1) + (num << (sizeof(num) - 1));
}


#undef HASH_MOD

#endif //__HASH_FUNCTIONS_H__
