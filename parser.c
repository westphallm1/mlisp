#include "keywords.h"
#include "mlisp.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ERR(msg)printf("%s\n",msg);return NULL;
#define MAX_ARGS 10
#define DEBUG

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
void * exec_prog(struct ast_node * node);

void do_sum(struct ast_node * node, void ** data, int argc){
    int * to_write;
    if(argc == 0){
        *data = malloc(sizeof(int));
        to_write = (int*)*data;
        *to_write = 0;
    }
    if(node == NULL)
        return;

    to_write = (int*)*data;
    void * arg_data = exec_prog(node);
    *to_write += *(int*)arg_data;
    free(arg_data);
}
void do_prod(struct ast_node * node, void ** data, int argc){
    int * to_write;
    if(argc == 0){
        *data = malloc(sizeof(int));
        to_write = (int*)*data;
        *to_write = 1;
    }
    if(node == NULL)
        return;

    to_write = (int*)*data;
    void * arg_data = exec_prog(node);
    *to_write *= *(int*)arg_data;
    free(arg_data);
}

/* use void * return type to return arbitrary data
 * expect higher level node to know what to do with it
 */
void * exec_prog(struct ast_node * node){
    void * data;
    if(node->is_root && node->next != NULL){
        if(!node->next->is_atom){
            ERR("Tuples must start with command");
        } 
        struct atom * cmd_atom = node->next->atom_child;
        
        struct ast_node * curr = node->next->next;
        int argc = 0;
        do {
            // do stuff based on 
            if(
            do_sum(curr,&data,argc++);
            curr = curr->next;
        } while(curr != NULL);

        return data;
    } else if(!node->is_atom && node->list_child->is_root){
        return exec_prog(node->list_child);
    }
    data = malloc(sizeof(int));
    int * int_data = (int *)data;
    *int_data = 2;
    return data;
}

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

int main(int argc, char * argv[]){
    char buffr[255];
    FILE * fp;
    if(argc < 2 || (fp = fopen(argv[1],"r")) == NULL){
        printf("Usage: %s FILE\n",argv[0]);
        return 1;
    }
    fread(buffr,255,1,fp);
    char * saveptr;
    struct ast_node * node = build_tree(buffr,&saveptr);
#ifdef DEBUG
    print_prog(node);
#endif /* DEBUG */
    int * result = (int *)exec_prog(node);
    printf("%d\n",*result);
    
}

