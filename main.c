#include <stack.h>
#include <core.h>
#include <tbstd.h>

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

int main(void) {
    struct env env;
    env_init(&env, 0x1000);

    struct word *program[] = {
        &core_lit, (struct word *)1,
        &core_lit, (struct word *)2,
        &core_add,
        &core_lit, (struct word *)4,
        &core_exit
    };

    env_load(&env, program);
    NEXT(&env);

    // we never return to main after entering the code, so no need for cleanup :^)

    return 0;
}
