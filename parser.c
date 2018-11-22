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
        ERR("Maximum AST nodes exceeded");
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

void __build_call_tree(struct ast_node * curr, char ** saveptr){
    uint32_t token;
    /* clear the leading '(' */
    get_token_r(NULL,saveptr);
    /* build the rest of the tree */
    while((token = peek_token_r(NULL,saveptr)) != RPAREN){
        curr -> next = build_tree(NULL,saveptr);
        curr = curr -> next;
    }
    /* null terminate */
    curr -> next = NULL;
    /* clear the trailing ')' */
    get_token_r(NULL,saveptr);
}

void __build_cflow_tree(struct ast_node * curr, char ** saveptr){
    uint32_t token;
    /* consume the leading ':' token */
    if(get_token_r(NULL,saveptr) != ':'){
        ERR("Expected ':'.");
    }
    /* get the conditional */
    curr -> next = build_tree(NULL,saveptr);
    /* get the rest of the statement as normal */
    curr = curr -> next;
    __build_call_tree(curr, saveptr);
    
}

struct ast_node * build_tree(char * stream, char **saveptr){
    uint32_t token, peek_token;
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
        ERR("Maximum AST nodes exceeded");
    }
    lnode -> root_type = 0;
    lnode -> next = NULL;

    peek_token = peek_token_r(NULL,saveptr);
    if(peek_token == LPAREN || token == IF || token == WHILE){
        atom = build_atom(token,stream,*saveptr);
        /*handle a list */
        if((lchild = alloc_entry()) == NULL){
            ERR("Maximum AST nodes exceeded");
        }
        curr = lchild;
        curr -> root_type = token;
        curr -> is_atom = 0;
        curr -> atom_child = NULL;
        /* set the atom up as the first child node */
        curr -> next = alloc_entry();
        curr = curr -> next;
        curr -> atom_child = atom;
        curr -> root_type = 0;
        curr -> is_atom = 1;
        /* special case for control flow */
        if(token == IF || token == WHILE){
            __build_cflow_tree(curr,saveptr);
        } else {
            __build_call_tree(curr,saveptr);
        }

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
