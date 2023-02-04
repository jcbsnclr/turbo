#include <stack.h>
#include <hash.h>
#include <core.h>

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

void env_init(struct env *env, size_t stack_size) {
    stack_init(&env->vs, stack_size);
    stack_init(&env->rs, stack_size);
    env->ip = NULL;

    env->dict = malloc(sizeof(struct dict));
    dict_init(env->dict);
}

void env_free(struct env *env) {
    stack_free(&env->vs);
    stack_free(&env->rs);
    env->ip = NULL;
    dict_free(env->dict);
    free(env->dict);
}

void env_load(struct env *env, struct word **code) {
    env->ip = code;
}

void dict_init(struct dict *dict) {
    dict->nslots = 64;
    dict->slots = malloc(sizeof(dict->slots[0]) * dict->nslots);
    memset(dict->slots, 0, sizeof(dict->slots[0]) * dict->nslots);
}

static void dict_node_free(struct dict_node *node) {
    if (node->prev)
        dict_node_free(node->prev);

    free(node->name);
    if (node->def.cleanup)
        node->def.cleanup(node->def.data);

    free(node);
}

void dict_free(struct dict *dict) {
    for (size_t i = 0; i < dict->nslots; i++)
        if (dict->slots[i])
            dict_node_free(dict->slots[i]);
}

void dict_define(struct dict *dict, char *name, wordimpl_t impl, void *data, cleanup_t cleanup) {
    hash_t hash = hash_str(name);
    size_t slot = hash % dict->nslots;
    struct dict_node *prev = dict->slots[slot];

    struct dict_node *node = malloc(sizeof(struct dict_node));

    node->hash = hash;
    node->name = strdup(name);
    node->prev = prev;

    node->def.impl    = impl;
    node->def.data    = data;
    node->def.cleanup = cleanup;

    dict->slots[slot] = node;
}

struct word *dict_lookup(struct dict *dict, char *name) {
    hash_t hash = hash_str(name);
    size_t slot = hash % dict->nslots;
    struct dict_node *cur = dict->slots[slot];

    while (cur) {
        if (cur->hash == hash && strcmp(cur->name, name) == 0)
            return &cur->def;

        cur = cur->prev;
    }

    return NULL;
}

void docol(struct env *env, void *data) {
    bool error;
    stack_push(&env->rs, (uint64_t)env->ip, &error);
   
    assert(!error);

    env->ip = (struct word **)data;
    NEXT(env);
}

void ret(struct env *env, void *data) {
    (void)data;
    bool error;

    env->ip = (struct word **)stack_pop(&env->rs, &error);

    assert(!error);

    NEXT(env);
}
