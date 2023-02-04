#include <tbstd.h>

#include <core.h>
#include <stack.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

struct tbstd_worddef {
    char *name;
    wordimpl_t impl;
};

// tbcore - virtual machine primitives
static void lit_impl(struct env *env, void *data) {
    (void)data;

    bool error;

    uint64_t v = (uint64_t)(*env->ip++);
    stack_push(&env->vs, v, &error);

    assert(!error);

    NEXT(env);
}

static void swap_impl(struct env *env, void *data) {
    (void)data;

    bool error;

    uint64_t v1, v2;
    v2 = stack_pop(&env->vs, &error);
    v1 = stack_pop(&env->vs, &error);

    assert(!error);

    stack_push(&env->vs, v2, &error);
    stack_push(&env->vs, v1, &error);

    assert(!error);

    NEXT(env);
}

static void dup_impl(struct env *env, void *data) {
    (void)data;

    bool error;

    uint64_t v = stack_pop(&env->vs, &error);

    assert(!error);

    stack_push(&env->vs, v, &error);
    stack_push(&env->vs, v, &error);

    assert(!error);

    NEXT(env);
}

static void over_impl(struct env *env, void *data) {
    (void)data;

    bool error;

    uint64_t v1, v2;
    v2 = stack_pop(&env->vs, &error);
    v1 = stack_pop(&env->vs, &error);

    assert(!error);

    stack_push(&env->vs, v1, &error);
    stack_push(&env->vs, v2, &error);
    stack_push(&env->vs, v1, &error);
 
    assert(!error);

    NEXT(env);
}

static void rot_impl(struct env *env, void *data) {
    (void)data;

    bool error;

    uint64_t v1, v2, v3;
    v3 = stack_pop(&env->vs, &error);
    v2 = stack_pop(&env->vs, &error);
    v1 = stack_pop(&env->vs, &error);

    assert(!error);

    stack_push(&env->vs, v2, &error);
    stack_push(&env->vs, v3, &error);
    stack_push(&env->vs, v1, &error);

    assert(!error);

    NEXT(env);
}

static void drop_impl(struct env *env, void *data) {
    (void)data;

    bool error;
    stack_pop(&env->vs, &error);

    assert(!error);

    NEXT(env);
}

static void ret_impl(struct env *env, void *data) {
    (void)data;
    bool error;

    env->ip = (struct word **)stack_pop(&env->rs, &error);

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

    env_free(env);

    exit(0);
}

static struct tbstd_worddef tbcore[] = {
    { .name = "lit",  .impl = lit_impl },

    { .name = "swap", .impl = swap_impl },
    { .name = "dup",  .impl = dup_impl },
    { .name = "over", .impl = over_impl },
    { .name = "rot",  .impl = rot_impl },
    { .name = "drop", .impl = drop_impl },

    { .name = "ret",  .impl = ret_impl },
    { .name = "exit", .impl = exit_impl },

    { NULL }
};

// tbmath - mathematical operations. integer values only currently
#define TBBINOP(name, op) \
    static void name (struct env *env, void *data) { \
        (void)data; \
        bool error; \
        uint64_t v1, v2; \
        v2 = stack_pop(&env->vs, &error); \
        v1 = stack_pop(&env->vs, &error); \
        assert(!error); \
        stack_push(&env->vs, v1 op v2, &error); \
        assert(!error); \
        NEXT(env); \
    }

TBBINOP(add_impl, +)
TBBINOP(sub_impl, -)
TBBINOP(mul_impl, *)
TBBINOP(div_impl, /)
TBBINOP(mod_impl, %)

static struct tbstd_worddef tbmath[] = {
    { .name = "+", .impl = add_impl },
    { .name = "-", .impl = sub_impl },
    { .name = "*", .impl = mul_impl },
    { .name = "/", .impl = div_impl },
    { .name = "%", .impl = mod_impl },
    { NULL }
};

static void load_mod(struct dict *dict, struct tbstd_worddef *mod) {
    struct tbstd_worddef *cur = mod;

    while (cur->name) {
        dict_define(dict, cur->name, cur->impl, NULL, NULL);
        cur++;
    }
}

void tbstd_init(struct dict *dict, enum tbstd_world world) {
    if (world & TBCORE)
        load_mod(dict, tbcore);
    if (world & TBMATH)
        load_mod(dict, tbmath);
}
