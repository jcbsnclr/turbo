#pragma once

#include <stdint.h>
#include <stddef.h>

typedef uint64_t hash_t;

hash_t hash_buf(char *data, size_t len);
hash_t hash_str(char *str);
