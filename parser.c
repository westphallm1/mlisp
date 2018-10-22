#include "keywords.h"
#include "mlisp.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_ARGS 10

#ifdef DEBUG

void print_prog(struct ast_node * node){
    while(node != NULL){
        if(node -> is_root){
            printf(":");
        } else if(node -> is_atom){
            if(node -> atom_child !=NULL){
                printf("%.*s ",node->atom_child->len,node->atom_child->strval);
            } 
        } else {
            printf("(");
            print_prog(node->list_child);
            printf(") ");
        }
        node = node->next;
    }
}
#endif /*DEBUG*/

struct atom * build_atom(uint32_t token, char * start, char * stop){
    struct atom * atom = malloc(sizeof (struct atom));
    if(atom == NULL){
        ERR("Out of memory");
    }
    atom -> token = token;
    while(*start <= ' ')
        start++;
    atom -> strval = start;
    atom -> len = stop - start;
    return atom;
}

struct ast_node * build_tree(char * stream, char **saveptr){
    uint32_t token;
    struct ast_node * lnode, *curr, *lchild;
    struct atom * atom;
    if(stream == NULL){
        stream = *saveptr;
    }
    token = get_token_r(stream, saveptr); 
    if(token == 0){
        /* nothing to parse */
        return NULL;
    }

    if((lnode = malloc(sizeof (struct ast_node))) == NULL){
        ERR("Out of memory");
    }
    lnode -> is_root = 0;
    lnode -> next = NULL;
    
    if(token == LPAREN){
        /*handle a list */
        if((lchild = malloc(sizeof (struct ast_node))) == NULL){
            ERR("Out of memory");
        }
        curr = lchild;
        curr -> is_root = 1;
        curr -> is_atom = 0;
        curr -> atom_child = NULL;
        while((token = peek_token_r(NULL,saveptr)) != RPAREN){
            curr -> next = build_tree(NULL,saveptr);
            curr = curr -> next;
        }
        /* null terminate */
        curr -> next = NULL;
        /* clear the trailing ')' */
        get_token_r(NULL,saveptr);

        lnode -> list_child = lchild;
        lnode -> is_root = 0;
        lnode -> is_atom = 0;
    } else if (token == RPAREN){
        /*shouldn't be encountered outside list */
        ERR("Unexpected ')'");
    } else {
        /* handle an atom */
        atom = build_atom(token,stream,*saveptr);
        lnode -> atom_child = atom;
        lnode -> is_root = 0;
        lnode -> is_atom = 1;
    }

    return lnode;
}
