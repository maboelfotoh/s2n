/*
http://stackoverflow.com/questions/98153/whats-the-best-hashing-algorithm-to-use-on-a-stl-string-when-using-hash-map
*/
#include "util.h"

unsigned int
hash(
    const char* s,
    unsigned int seed = 0)
{
    unsigned hash = seed;
    while (*s)
    {
        hash = hash * 101  +  *s++;
    }
    return hash;
}
