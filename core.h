#pragma once

#include <stack.h>

#include <stddef.h>

#define NEXT(envptr) { \
    struct word *w = (*(envptr)->ip++); \
    w->interp((envptr), w->data); \
}

struct env {
    // value stack
    struct stack vs;
    // return stack
    struct stack rs;
    struct word **ip;
};

typedef void (*wordimpl_t)(struct env *env, void *data);

struct word {
    wordimpl_t interp;
    void *data;
};

void env_init(struct env *env, size_t stack_size);
void env_free(struct env *env);

void env_load(struct env *env, struct word **code);
