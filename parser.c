#include "keywords.h"
#include "mlisp.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

union ast_entry{
    struct atom atom;
    struct ast_node node;
};
#ifdef STATIC

union ast_entry ENTRIES[STATIC_MAX_NODES];
union ast_entry * entry_ptr = ENTRIES;

void * alloc_entry(){
    if(++entry_ptr - ENTRIES == STATIC_MAX_NODES)
        return NULL;
    return entry_ptr - 1;
}
void free_ast(){
    entry_ptr = ENTRIES;
}

#else

void * alloc_entry(){
    return malloc(sizeof(ast_entry));
}
void free_ast(){
}

#endif /* STATIC */


#ifdef DEBUG
void print_prog(struct ast_node * node){
    while(node != NULL){
        if(node -> root_type){
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
    struct atom * atom = alloc_entry(); 
    if(atom == NULL){
        ERR("Out of memory");
    }
    atom -> token = token;
    while(*start <= ' ')
        start++;
    atom -> strval = start;
    atom -> len = stop - start;
    if(token == STRLIT){
        atom->strval += 1;
        atom -> len -= 2;
    }
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

    if((lnode = alloc_entry()) == NULL){
        ERR("Out of memory");
    }
    lnode -> root_type = 0;
    lnode -> next = NULL;
    
    if(token == LPAREN){
        /*handle a list */
        if((lchild = alloc_entry()) == NULL){
            ERR("Out of memory");
        }
        curr = lchild;
        curr -> root_type = peek_token_r(NULL,saveptr);
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
        lnode -> root_type = 0;
        lnode -> is_atom = 0;
    } else if (token == RPAREN){
        /*shouldn't be encountered outside list */
        ERR("Unexpected ')'");
    } else {
        /* handle an atom */
        atom = build_atom(token,stream,*saveptr);
        lnode -> atom_child = atom;
        lnode -> root_type = 0;
        lnode -> is_atom = 1;
    }

    return lnode;
}
