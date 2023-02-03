#include <stack.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

void stack_init(struct stack *st, size_t cap) {
    st->data = malloc(sizeof(uint64_t) * cap);
    st->cap  = cap;
    st->ptr  = 0;
}

void stack_free(struct stack *st) {
    assert(st->data);
    free(st->data);

    st->data = NULL;
    st->cap  = 0;
    st->ptr  = 0;
}

void stack_push(struct stack *st, uint64_t val, bool *error) {
    assert(st->data);
    *error = false;

    if (st->ptr < st->cap) {
        *error = false;
        st->data[st->ptr++] = val;
    } else {
        *error = true;
    }
}

uint64_t stack_pop(struct stack *st, bool *error) {
    assert(st->data);

    if (st->ptr > 0) {
        *error = false;
        return st->data[--st->ptr];
    } else {
        *error = true;
        return 0;
    }
}
