#include <stack.h>

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

int main(void) {
    bool error = false;
    struct stack st;
    stack_init(&st, 5);

    stack_push(&st, 1, &error);
    stack_push(&st, 2, &error);
    stack_push(&st, 3, &error);

    for (size_t i = 0; i < 3; i++)
        printf("num: %ld\n", stack_pop(&st, &error));

    for (size_t i = 0; i < 5; i++)
        stack_push(&st, i, &error);

    assert(!error);

    stack_push(&st, 123, &error);

    assert(error);

    stack_free(&st);

    return 0;
}
