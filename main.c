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

    // load TBCORE and TBMATH standard word libraries
    tbstd_init(env.dict, TBCORE | TBMATH);

    // lookup words in dictionary that we will use
    struct word *lit, *add, *mul, *ret, *exit, *add2;
    lit = dict_lookup(env.dict, "lit");
    add = dict_lookup(env.dict, "+");
    mul = dict_lookup(env.dict, "*");
    ret = dict_lookup(env.dict, "ret");
    exit = dict_lookup(env.dict, "exit");

    // this is the "data" for the add2 word, which will be accessed via add2's interpreter.
    // every word consists of an interpreter function ref and data pointer.
    // in FORTH: : 2 + ;
    struct word *add2d[] = {
        LIT(2), add, ret
    };

    // define add2 in the dictionary and get a pointer to it's definition.
    // 
    // we use "docol" (do colon, from FORTH), defined in core.c, as our
    // interpreter. "docol" simply pushes the current instruction pointer to
    // the return stack, and then loads the word's "data" field into the instruction
    // pointer before running the next word.
    // .
    // the inverse of "docol" (roughly the "call" op in most ISAs) is "ret", which pops a value
    // from the top of the return stack and loads it into the instruction pointer. 
    // ..
    //          dict      name    interp data   cleanup
    dict_define(env.dict, "add2", docol, add2d, NULL);
    add2 = dict_lookup(env.dict, "add2");

    // in FORTH: 1 add2 2 * 
    struct word *program[] = {
        LIT(1), add2,
        LIT(2), mul, exit
    };

    // load the program into the virtual machine and call NEXT (really a macro), which fetches the
    // next word from the instruction pointer, increments the instruction pointer, and then
    // passes the word's data pointer into it's interpreter.
    //
    // in the case of native words - e.g. add - the data field will be a nullptr, and the interpreter
    // will be the native implementation of add (in this case, add_impl in tbstd.c).
    // 
    // in the case of user-defined words (e.g. in this case add2), the data field will be a list of
    // words to execute, and the interpreter will be set to docol. 
    env_load(&env, program);
    NEXT(&env);

    // as the virtual machine never returns to main, cleanup is performed in the "exit" word.
    // see: tbstd.c

    return 0;
}
