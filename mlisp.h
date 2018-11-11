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


/* lexer.h */
int get_token_r(char * stream, char ** saveptr);
int peek_token_r(char * stream, char ** saveptr);
/* parser.h */
struct ast_node * build_tree(char * stream, char **saveptr);
void free_ast(void);

#ifdef DEBUG
void print_prog(struct ast_node * node);
#endif /*DEBUG*/
#endif /*MLISP_H*/
