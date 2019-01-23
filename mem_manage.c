/* Custom stack-like memory allocator for tighter control */
#include "keywords.h"
#include "slimp.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct var_t * VARS;
char * STR_STACK;
char * STACK;
char * sp;
char * str_sp;
char ** PROG_ARGV;
int PROG_ARGC;
int stack_size = STATIC_STACK_SIZE;

union ast_entry{
    struct atom atom;
    struct ast_node node;
};

union ast_entry ENTRIES[512];
union ast_entry * entry_ptr = ENTRIES;

void * alloc_entry(){
    return entry_ptr++;
}
void free_ast(){
    entry_ptr = ENTRIES;
}

void stack_setup(){
    VARS=sbrk(STATIC_NVARS*sizeof(struct var_t));
    STR_STACK=sbrk(STATIC_STR_STACK_SIZE);
    STACK=sbrk(STATIC_STACK_SIZE);
    sp = STACK;
    str_sp = STR_STACK;
}

void * s_push(size_t size){
    sp += size;
    if(sp > STACK + stack_size){
        stack_size += STATIC_STACK_STEP;
        sbrk(STATIC_STACK_STEP);
    }
    return sp - size;
}

void s_pop(size_t size){
    if(sp > STACK + stack_size){
        stack_size -= STATIC_STACK_STEP;
        sbrk(-STATIC_STACK_STEP);
    }
    sp -= size;
}

void * s_str_push(size_t size){
    str_sp += size;
    return str_sp - size;
}

void s_str_pop(size_t size){
    str_sp -= size;
}
