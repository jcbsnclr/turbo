#pragma once

#include <stack.h>
#include <hash.h>

#include <stddef.h>

#define NEXT(envptr) { \
    struct word *w = (*(envptr)->ip++); \
    w->impl((envptr), w->data); \
}

typedef void (*cleanup_t)(void *);

struct env {
    // value stack
    struct stack vs;
    // return stack
    struct stack rs;
    struct word **ip;
    struct dict *dict;
};

typedef void (*wordimpl_t)(struct env *env, void *data);

struct word {
    wordimpl_t impl;
    void *data;
    cleanup_t cleanup;
};

struct dict_node {
    hash_t hash;
    char *name;
    struct word def;
    struct dict_node *prev;
};

struct dict {
    size_t nslots;
    struct dict_node **slots;
};

void env_init(struct env *env, size_t stack_size);
void env_free(struct env *env);

void env_load(struct env *env, struct word **code);

void dict_init(struct dict *dict);
void dict_free(struct dict *dict);

void dict_define(struct dict *dict, char *name, wordimpl_t impl, void *data, cleanup_t cleanup);
struct word *dict_lookup(struct dict *dict, char *name);

void docol(struct env *env, void *data);
