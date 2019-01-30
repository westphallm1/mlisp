/* Custom stack-like memory allocator
 * Mainly implemented for the challenge, provides little to no practical
 * advantage and may in fact perform worse than standard malloc/free ussage
 */
#include "keywords.h"
#include "slimp.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h> 

struct var_t * VARS;
char * STR_STACK;
char * STACK;
char * sp;
char * str_sp;
int stack_size = STATIC_STACK_SIZE;

union ast_entry{
    struct atom atom;
    struct ast_node node;
};

union ast_entry * ENTRIES;
union ast_entry * entry_ptr;// = ENTRIES;

/* determine if a character sequence corresponds to a variable */
int __var_name_match(char * id, struct var_t * v){
    return !memcmp(id, v->id,4);
}
/* find the location of a variable in the variable table */
struct var_t * var_by_name(struct atom * atom){
    int hash_pos = 0;
    int shift = 0;
    char id[4] = {0,0,0,0};
    memcpy(id,atom->strval,(atom->len<4)?atom->len:4);
    int token = (id[0]+(id[1]<<7)+(id[2]<<14)+(id[3]<<21))%STATIC_NVARS;
    int token0 = token;
    do{
        if((!VARS[token].type)){
            memcpy(VARS[token].id,id,4);
            return &VARS[token];
        }
        if(__var_name_match(id,&VARS[token])){
            return &VARS[token];
        }
        token = (token+1)%STATIC_NVARS;

    }while(token != token0);
    ERR("No more space for static variables.");
}

/* mem map a file to the top of the stack 
 * should probably be called before anything else
 * https://stackoverflow.com/a/238607
 */
char * mem_map(FILE * fp){
    fseek(fp, 0, SEEK_END);
    size_t sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char * file_start = sbrk(sz+1);
    fread(file_start,sz,1,fp);
    return file_start;
}

void stack_setup(){
    /* allocate fixed space for variables at bottom of stack */
    VARS=sbrk(STATIC_NVARS*sizeof(struct var_t));
    /* then fixed space for string literals */
    STR_STACK=sbrk(STATIC_STR_STACK_SIZE);
    /* then fixed space for AST nodes */
    str_sp = STR_STACK;
    ENTRIES = sbrk(STATIC_STACK_SIZE);
    entry_ptr = ENTRIES;
    /* then expanding space for work stack */
    //STACK=sbrk(STATIC_STACK_SIZE);
    //sp = STACK;
}

void reset_stack(){
    sp = STACK;
}

void * alloc_entry(){
    entry_ptr++;
    if ((void *) entry_ptr > stack_size + (void *) ENTRIES){
        stack_size += STATIC_STACK_STEP;
        sbrk(STATIC_STACK_STEP);
    }
    /* keep the start of the work stack at the top of the AST nodes */
    STACK = (char *) entry_ptr;
    sp = STACK;
    return entry_ptr - 1;
}

void free_ast(){
    entry_ptr = ENTRIES;
}


void * s_push(size_t size){
    sp += size;
    if(sp > stack_size + (char *) ENTRIES){
        stack_size += STATIC_STACK_STEP;
        sbrk(STATIC_STACK_STEP);
    }
    return sp - size;
}

void * push_var_t(){
    return s_push(sizeof(struct var_t));
}

void s_pop(size_t size){
    sp -= size;
}

void * s_str_push(size_t size){
    str_sp += size;
    return str_sp - size;
}

void s_str_pop(size_t size){
    str_sp -= size;
}
enum mem_region mem_region_of_ptr(char * ptr){
    if(ptr >= STACK && ptr <= STACK + stack_size){
        return STACK_MEM;
    } else if (ptr >= STR_STACK && 
               ptr <= STR_STACK + STATIC_STR_STACK_SIZE){
        return STRING_MEM;
    }
}
