#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct stack {
    uint64_t *data;
    size_t cap;
    size_t ptr;
};

void stack_init(struct stack *st, size_t cap);
void stack_free(struct stack *st);

void stack_push(struct stack *st, uint64_t val, bool *error);
uint64_t stack_pop(struct stack *st, bool *error);
