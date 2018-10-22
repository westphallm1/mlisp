#include <stdint.h>

#ifndef MLISP_H
#define MLISP_H

#define DEBUG
#define ERR(msg)printf("%s\n",msg);return NULL;

struct ast_node {
    struct ast_node * next; /* Next argument in the list */
    char is_atom; /* arguments can be either atom or list */
    char is_root; /* is this node the start of a command? */
    struct ast_node * list_child; /* if argument is a list */
    struct atom * atom_child; /* if argument is an atom */
};

struct atom {
    uint32_t token;
    int ival;
    float fval;
    char * strval;
    int len;
};


int get_token_r(char * stream, char ** saveptr);
int peek_token_r(char * stream, char ** saveptr);


#ifdef DEBUG
void print_prog(struct ast_node * node);
#endif /*DEBUG*/
#endif /*MLISP_H*/
