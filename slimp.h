#include <stdio.h>
#include <stdint.h>

#ifndef MLISP_H
#define MLISP_H
#include "config.h"

#define ERR(msg)printf("%s\n",msg);exit(1);

struct ast_node {
    char is_atom; /* arguments can be either atom or list */
    unsigned char root_type; /* is this node the start of a command? */
    union {
        struct ast_node * list_child; /* if argument is a list */
        struct atom * atom_child; /* if argument is an atom */
    };
    struct ast_node * next; /* Next argument in the list */
};

struct atom {
    unsigned char token;
    char len;
    char * strval;
};

struct var_t {
    char id[4];
    unsigned char type;
    union{
        int i;
        float f;
        char * s;
    };
};

/* lexer.c */
int get_token_r(char * stream, char ** saveptr);
int peek_token_r(char * stream, char ** saveptr);
int peek_newl(char * stream);
int get_stmt(char * buffr, int max_len, FILE * stream);
/* parser.c */
struct ast_node * build_tree(char * stream, char **saveptr);
void free_ast(void);

/* mem_manage.c */
void * alloc_entry();
void free_ast();
void stack_setup();
void * s_push(size_t size);
void s_pop(size_t size);
void * s_str_push(size_t size);
void s_str_pop(size_t size);

extern struct var_t * VARS;
extern char * STR_STACK;
extern char * STACK;
extern char * sp;
extern char * str_sp;
extern char ** PROG_ARGV;
extern int PROG_ARGC;
extern int stack_size;

#ifdef DEBUG
void print_prog(struct ast_node * node);
#endif /*DEBUG*/
#endif /*MLISP_H*/
