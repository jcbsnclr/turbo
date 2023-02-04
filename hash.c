#include <hash.h>

#include <stdint.h>
#include <stddef.h>
#include <string.h>

static const uint64_t FNV_OFFSET_BASIS = 0xcbf29ce484222325;
static const uint64_t FNV_PRIME        = 0x100000001b3;

hash_t hash_buf(char *data, size_t len) {
    hash_t hash = FNV_OFFSET_BASIS;

    for (size_t i = 0; i < len; i++) {
        hash ^= data[i];
        hash *= FNV_PRIME;
    }

    return hash;
}

hash_t hash_str(char *str) {
    return hash_buf(str, strlen(str));
}
