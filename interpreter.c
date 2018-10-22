#include "keywords.h"
#include "mlisp.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
            if(cmd_atom -> token == PLUS)
                do_sum(curr,&data,argc++);
            else if(cmd_atom -> token == TIMES)
                do_prod(curr,&data,argc++);
            curr = curr->next;
        } while(curr != NULL);

        return data;
    } else if(!node->is_atom && node->list_child->is_root){
        return exec_prog(node->list_child);
    } else if(node->is_atom) {
        data = malloc(sizeof(int));
        int * int_data = (int *)data;
        *int_data = 2;
        return data;
    } else {
        ERR("Unknown node type");
    }
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

