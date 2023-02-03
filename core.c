#include <stack.h>
#include <core.h>

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

static void lit_impl(struct env *env, void *data) {
    (void)data;
    bool error;

    stack_push(&env->vs, (uint64_t)(*env->ip++), &error);

    assert(!error);

    NEXT(env);
}

static void add_impl(struct env *env, void *data) {
    (void)data;
    bool error;

    uint64_t v1, v2;
    v2 = stack_pop(&env->vs, &error);
    v1 = stack_pop(&env->vs, &error);

    assert(!error);

    stack_push(&env->vs, v1 + v2, &error);

    assert(!error);

    NEXT(env);
}

static void exit_impl(struct env *env, void *data) {
    (void)data;

    fprintf(stderr, "Exit...\nstack: [");

    for (size_t i = 0; i < env->vs.ptr; i++) {
        fprintf(stderr, "%ld", env->vs.data[i]);

        if (i != env->vs.ptr - 1)
            fprintf(stderr, ", ");
    }

    fprintf(stderr, "]\n");

    exit(0);
}

struct word core_lit = { .interp = lit_impl, .data = NULL };
struct word core_add = { .interp = add_impl, .data = NULL };
struct word core_exit = { .interp = exit_impl, .data = NULL };

void env_init(struct env *env, size_t stack_size) {
    stack_init(&env->vs, stack_size);
    stack_init(&env->rs, stack_size);
    env->ip = NULL;
}

void env_free(struct env *env) {
    stack_free(&env->vs);
    stack_free(&env->rs);
    env->ip = NULL;
}

void env_load(struct env *env, struct word **code) {
    env->ip = code;
}
