#include "keywords.h"
#include "mlisp.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define ERR(msg)printf("%s\n",msg);exit(1);

struct ast_node {
    struct ast_node * next; /* Next argument in the list */
    char is_atom; /* arguments can be either atom or list */
    struct ast_node * list_child; /* if argument is a list */
    struct atom * atom_child; /* if argument is an atom */
};

struct atom {
    uint32_t token;
    int ival;
    float fval;
    char * strval;
};


struct atom * build_atom(uint32_t token){
    struct atom * atom = malloc(sizeof (struct atom));
    if(atom == NULL){
        ERR("Out of memory");
    }
    atom -> token = token;
    return atom;
}
struct ast_node * build_tree(char * stream, char **saveptr){
    uint32_t token;
    struct ast_node * lnode, *curr;
    struct atom * atom;
    token = get_token_r(stream, saveptr); 
    if(token == 0){
        ERR("Unexpected EOF");
    }

    if((lnode = malloc(sizeof (struct ast_node))) == NULL){
        ERR("Out of memory");
    }
    
    if(token == LPAREN){
        /*handle a list */
        lnode -> is_atom = 0;
        curr = lnode;
        while((token = peek_token_r(NULL,saveptr)) != RPAREN){
            curr -> next = build_tree(*saveptr,saveptr);
            curr = curr -> next;
        }
        /* null terminate */
        curr -> next = NULL;
        get_token_r(NULL,saveptr);
    } else if (token == RPAREN){
        /*shouldn't be encountered outside list */
        ERR("Unexpected ')'");
    } else {
        /* handle an atom */
        atom = build_atom(token);
        lnode -> atom_child = atom;
        lnode -> is_atom = 1;
    }

    return lnode;
}

int main(){
    char * lex_me = "(! hello \"hello world\" world)";
    char * saveptr;
    struct ast_node * node = build_tree(lex_me,&saveptr);
    

}

