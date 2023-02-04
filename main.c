#include <stack.h>
#include <core.h>
#include <tbstd.h>

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define LIT(n) lit, (struct word *)n

int main(void) {
    struct env env;
    env_init(&env, 0x1000);

    tbstd_init(env.dict, TBCORE | TBMATH);

    struct word *lit, *add, *exit;

    lit = dict_lookup(env.dict, "lit");
    add = dict_lookup(env.dict, "+");
    exit = dict_lookup(env.dict, "exit");

    struct word *program[] = {
        LIT(1), LIT(2), add,
        LIT(4), exit
    };

    env_load(&env, program);
    NEXT(&env);

    // we never return to main after entering the code, so no need for cleanup :^)

    return 0;
}
