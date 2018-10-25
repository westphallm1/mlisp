#include "keywords.h"
#include "mlisp.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



int VARS[26];
char STACK[1024];
char * sp = STACK;

void * s_push(size_t size){
    sp += size;
    return sp - size;
}

void s_pop(size_t size){
    sp -= size;
}

void * exec_prog(struct ast_node * node,int context);

#define MATH_FUNCTION(name,initial,operation) \
void do_##name(struct ast_node * node, void ** data, int argc, int context){\
    int * to_write; \
    if(node == NULL) \
        return; \
    if(argc == 0){ \
        *data = s_push(sizeof(int)); \
        to_write = (int*)*data; \
        *to_write = initial ; \
    } \
    to_write = (int*)*data; \
    void * arg_data = exec_prog(node,context); \
    int operand = *(int*)arg_data; \
    operation ;\
    s_pop(sizeof(int)); \
}


void do_assign(struct ast_node * node, void ** data, int argc, int context){
    if(argc == 0){
        //get the address to write
        *data = exec_prog(node,context);
    } else {
        //write the address
        *(int *)*data = *(int *)exec_prog(node,context);
    }
}

int atoi_l(struct atom * atom_child){
    int value;
    char tmp_term;
    char * str;
    int len;
    str = atom_child -> strval;
    len = atom_child -> len;
    tmp_term = *(str + len);
    *(str + len) = '\0';
    value = atoi(str);
    *(str + len) = tmp_term;
    return value;
}

MATH_FUNCTION(sum,0, *to_write += operand);
MATH_FUNCTION(or,0, *to_write |= operand);
MATH_FUNCTION(and,~0, *to_write &= operand);
MATH_FUNCTION(not,0, *to_write = ~operand);
MATH_FUNCTION(negate,0, *to_write = !operand);
MATH_FUNCTION(prod,1, *to_write *= operand);
MATH_FUNCTION(minus,0,switch(argc){
    case 0:
        *to_write = - operand;
        break;
    case 1:
        *to_write = -*to_write - operand;
        break;
    default:
        *to_write = *to_write - operand;
});
MATH_FUNCTION(divide,0,*to_write = (argc == 0)?operand: *to_write / operand);

void (*operators[])(struct ast_node*,void**,int,int) = {
    [PLUS] = do_sum,
    [TIMES] = do_prod,
    [MINUS] = do_minus,
    [DIVIDE] = do_divide,
    [OR] = do_or,
    [AND] = do_and,
    [NOT] = do_not,
    [NEGATE] = do_negate,
    [ASSIGN] = do_assign,
};


void * handle_int(struct atom * atom, int context){
    void * data = s_push(sizeof(int));
    int * int_data = (int *)data;
    *int_data = atoi_l(atom);
    return data;
}

void * handle_id(struct atom * atom, int context){
    if(context == ASSIGN){
        //return the address of the variable
        return (void *)&VARS[*atom->strval-'a'];
    } else {
        //push the variable's value to the stack
        int to_push = VARS[*atom->strval-'a'];
        int * push_addr = (int *) s_push(sizeof(int));
        *push_addr = to_push;
        return (void *)push_addr;
    }
}

void * (*atom_handlers[])(struct atom*,int) = {
    [INT] = handle_int,
    [ID] = handle_id
};
/* use void * return type to return arbitrary data
 * expect higher level node to know what to do with it
 */
void * exec_prog(struct ast_node * node, int context){
    if(node->is_root && node->next != NULL){
        void * data;
        if(!node->next->is_atom){
            ERR("Tuples must start with command");
        } 
        struct atom * cmd_atom = node->next->atom_child;
        
        struct ast_node * curr = node->next->next;
        int argc = 0;
        do {
            operators[cmd_atom -> token](curr,&data,argc++,cmd_atom->token);
            curr = curr->next;
        } while(curr != NULL);

        return data;
    } else if(!node->is_atom && node->list_child->is_root){
        return exec_prog(node->list_child,context);
    } else if(node->is_atom) {
        return atom_handlers[node->atom_child->token](node->atom_child,context);
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
    while(node != NULL){
        if(node != NULL && ! node -> is_atom){
#ifdef DEBUG
            print_prog(node);
#endif /* DEBUG */
            int * result = (int *)exec_prog(node,-1);
            printf("%d\n",*result);
        }
        node = build_tree(saveptr,&saveptr);
    }
}

