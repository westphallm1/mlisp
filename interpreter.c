#include "keywords.h"
#include "mlisp.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct var_t {
    char id[4];
    unsigned char type;
    union{
        int i;
        float f;
        char * s;
    };
};

struct var_t VARS[STATIC_NVARS];
char STACK[STATIC_STACK_SIZE];
char STR_STACK[STATIC_STR_STACK_SIZE];
char * sp = STACK;
char * str_sp = STR_STACK;
char ** PROG_ARGV;
int PROG_ARGC;

void * s_push(size_t size){
    sp += size;
    return sp - size;
}

void s_pop(size_t size){
    sp -= size;
}

void * s_str_push(size_t size){
    str_sp += size;
    return str_sp - size;
}

void s_str_pop(size_t size){
    str_sp -= size;
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
MATH_FUNCTION(mod,0,*to_write = (argc == 0)?operand: *to_write % operand);
MATH_FUNCTION(divide,0,*to_write = (argc == 0)?operand: *to_write / operand);
MATH_FUNCTION(less,0,*to_write = (argc == 0)? operand: *to_write < operand);
MATH_FUNCTION(greater,0,*to_write = (argc == 0)? operand: *to_write > operand);
MATH_FUNCTION(le,0,*to_write = (argc == 0)? operand: *to_write <= operand);
MATH_FUNCTION(ge,0,*to_write = (argc == 0)? operand: *to_write >= operand);
MATH_FUNCTION(equals,0,*to_write = (argc == 0)? operand: *to_write == operand);

#define ASSIGN_FUNCTION(name,operation)\
void do_##name(struct ast_node * node, void ** data, int argc, int context){\
    if(argc == 0){\
        /*get the address to write*/\
        *data = exec_prog(node,context);\
    } else {\
        /*write the address*/\
        struct var_t * var = (struct var_t *)*data;\
        if(node->is_atom){\
            switch(node->atom_child->token){\
                case INT:\
                case FLOAT:\
                case STRLIT:\
                    var -> type = node->atom_child->token;\
                default:\
                    break;\
            }\
        }else {\
            switch(node->list_child->root_type){\
                case ARGV:\
                case SREAD:\
                    var -> type = STRLIT;\
                    break;\
                default:\
                    break;\
            }\
        }\
        switch(var->type){\
            case INT:\
                var->i operation *(int *)exec_prog(node,PLUS);\
                break;\
            case FLOAT:\
                var->f operation *(float *)exec_prog(node,PLUS);\
                break;\
            case STRLIT:\
                var->s = (char *) exec_prog(node,PLUS);\
                break;\
        }\
    }\
}
ASSIGN_FUNCTION(assign,=);
ASSIGN_FUNCTION(plusassign,+=);
ASSIGN_FUNCTION(timesassign,*=);
ASSIGN_FUNCTION(minusassign,-=);
ASSIGN_FUNCTION(divassign,/=);

/* get a string from stdin */
void do_sread(struct ast_node * node, void ** data, int argc, int context){
    /* get the head of the string stack */
    char * push_addr = s_str_push(0);
    /* don't let the string stack overflow */
    int max_len = STR_STACK + STATIC_STACK_SIZE - str_sp;
    if(node != NULL){
        max_len = *(int *)exec_prog(node,context);
        s_pop(sizeof(int));
    }
    if(node == NULL || node->next == NULL){
        fgets(push_addr,max_len,stdin);
        int len = strlen(push_addr);
        *(push_addr + len - 1) = 0;
        s_str_push(len);
        *data = push_addr;
    }

}
void do_strcmp(struct ast_node * node, void ** data, int argc, int context){
    if(argc == 0){
        *data = exec_prog(node,context);
    }else{
        char * arg1 = (char *) *data;
        char * arg2 = (char *) exec_prog(node,context);
        int * to_push = s_push(sizeof(int));
        *to_push = strcmp(arg1,arg2);
        *data = to_push;
    }
}
/* get arg at position as a string */
void do_argv(struct ast_node * node, void ** data, int argc, int context){
    if(argc == 0){
        char * to_push = PROG_ARGV[*(int *)exec_prog(node,context)];
        s_pop(sizeof(int));
        int len = strlen(to_push)+1;
        char * push_addr = s_str_push(len);
        memcpy(push_addr,to_push,len);
        *(push_addr + len) = 0;
        *data = (void *)push_addr;
    }
}

/* get number of arguments */
void do_argc(struct ast_node * node, void ** data, int argc, int context){
    if(argc == 0){
        *data = s_push(sizeof(int));
        *(int *)*data = PROG_ARGC;
    }
}
/*quit the program with return code*/
void do_exit(struct ast_node * node, void ** data, int argc, int context){
    int exit_code = *(int *)exec_prog(node,context);
    exit(exit_code);
}

void do_swrite(struct ast_node * node, void ** data, int argc, int context){
    void * arg_data = exec_prog(node,context);
    char * operand = (char *)arg_data;
    if(operand >= STACK && operand <= STACK + STATIC_STACK_SIZE){
        /* if the operator is pointing at an item on the stack,
         * print its numeric value */
        printf("%d ",*(int*)operand);
        s_pop(sizeof(int));
    } else if (operand >= STR_STACK && 
               operand <= STR_STACK + STATIC_STR_STACK_SIZE){
        /* if the operator is pointing at an item in string memory
         * print its value as a string */
        printf("%s",(char *)operand);
    }
    if(node->next == NULL){
        printf("\n");
        int * n_written = s_push(sizeof(int));
        *n_written = argc;
    }
}

void do_if(struct ast_node * node, void ** data, int argc, int context){
    if(argc == 0){
        //get the address to write
        *data = s_push(sizeof(int));
        *(int *)*data = *(int *)exec_prog(node,context);
        s_pop(sizeof(int));
    } else {
        //write the address
        int * to_write = (int *)*data;
        //else: flip the condition
        if(node->is_atom && node->atom_child->token == ELSE){
            *to_write = !*to_write;
            return;
        }
        if(*to_write){
            exec_prog(node,context);
            s_pop(sizeof(int));
        }
    }
}

void do_while(struct ast_node * node, void ** data, int argc, int context){
    if(argc == 0){
        //get the address to write
        *data = s_push(sizeof(int));
        *(int *)*data = *(int *)exec_prog(node,context);
    } else {
        //write the address
        int * to_write = (int *)*data;
        if(*to_write){
            exec_prog(node,context);
            s_pop(sizeof(int));
        }
    }
}

void do_toint(struct ast_node * node, void ** data, int argc, int context){
    void * arg_data = exec_prog(node,context);
    char * operand = (char *)arg_data;
    *data = s_push(sizeof(int));
    int* to_write = (int *)*data;
    *to_write = atoi(operand);
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


void (*operators[])(struct ast_node*,void**,int,int) = {
    [PLUS] = do_sum,
    [TIMES] = do_prod,
    [MINUS] = do_minus,
    [DIVIDE] = do_divide,
    [MOD] = do_mod,
    [OR] = do_or,
    [AND] = do_and,
    [NOT] = do_not,
    [NEGATE] = do_negate,
    [ASSIGN] = do_assign,
    [PLUSASSIGN] = do_plusassign,
    [TIMESASSIGN] = do_timesassign,
    [MINUSASSIGN] = do_minusassign,
    [DIVASSIGN] = do_divassign,
    [LESS] = do_less,
    [GREATER] = do_greater,
    [LESSEQ] = do_le,
    [GREATEREQ] = do_ge,
    [EQUALS] = do_equals,
    [WHILE] = do_while,
    [IF] = do_if,
    [ARGV] = do_argv,
    [ARGC] = do_argc,
    [STRCMP] = do_strcmp,
    [SREAD] = do_sread,
    [SWRITE] = do_swrite,
    [TOINT] = do_toint,
    [EXIT] = do_exit,
};


void * handle_int(struct atom * atom, int context){
    void * data = s_push(sizeof(int));
    int * int_data = (int *)data;
    *int_data = atoi_l(atom);
    return data;
}

struct var_t * __get_id_index(struct atom * atom){
    return &VARS[*atom->strval-'a'];
}
void * handle_id(struct atom * atom, int context){
    struct var_t * index;
    union {
     struct var_t * v;   
     int * i;   
     float * f;   
     char * s;   
    } push_addr;
    index = __get_id_index(atom);
    switch(context){
        case ASSIGN:
        case PLUSASSIGN:
        case MINUSASSIGN:
        case TIMESASSIGN:
        case DIVASSIGN:
            push_addr.v = index;
            if(!push_addr.v->type){
                push_addr.v -> type = INT;
            }
            return (void *)push_addr.v;
        default:
            break;
    }
    switch(index ->type){
        case INT:
            push_addr.i = (int *) s_push(sizeof(int));
            *push_addr.i = index->i;
            return (void *)push_addr.i;
        case FLOAT:
            push_addr.f = (float *) s_push(sizeof(float));
            *push_addr.f = index->f;
            return (void *)push_addr.f;
        case STRLIT:
            return (void *)index->s;
    }
}

void * handle_strlit(struct atom * atom, int context){
    char * push_addr = s_str_push(atom->len+1);
    *(push_addr+atom->len) = 0;
    memcpy(push_addr,atom->strval,atom->len);
    return (void *) push_addr;

}

void * (*atom_handlers[])(struct atom*,int) = {
    [INT] = handle_int,
    [ID] = handle_id,
    [STRLIT] = handle_strlit,
};
/* use void * return type to return arbitrary data
 * expect higher level node to know what to do with it
 */
void * exec_prog(struct ast_node * node, int context){
    if(node->root_type && node->next != NULL){
        void * data;
        if(!node->next->is_atom){
            ERR("Tuples must start with command");
        } 
        struct atom * cmd_atom = node->next->atom_child;
        
        struct ast_node * curr = node->next->next;
        struct ast_node * first_curr = curr;
        int argc = 0;
        do {
            operators[cmd_atom -> token](curr,&data,argc++,cmd_atom->token);
            if(curr != NULL)
                curr = curr->next;
            // special case for while loops: go back to the condition check node
            if(curr == NULL && cmd_atom -> token == WHILE && *(int *)data){
                curr = first_curr;
                argc = 0;
            }
        } while(curr != NULL);

        //clear the stack after the root has finished
        if(context == -1){
            sp = STACK;
        }
        return data;
    } else if(!node->is_atom && node->list_child->root_type){
        return exec_prog(node->list_child,context);
    } else if(node->is_atom) {
        return atom_handlers[node->atom_child->token](node->atom_child,context);
    } else {
        ERR("Unknown node type");
    }
}

int main(int argc, char ** argv){
    char buffr[INPUT_BUFFR_SIZE];
    FILE * fp;
    int * result;
    char * saveptr;
    if(argc < 2 || (fp = fopen(argv[1],"r")) == NULL){
        printf("Usage: %s FILE\n",argv[0]);
        return 1;
    }
    /*pass argc and argv to interpretted program*/
    PROG_ARGV = argv + 2;
    PROG_ARGC = argc - 2;
    /*read program in from file (up to 1 kb)*/
    fread(buffr,1024,1,fp);

    struct ast_node * node = build_tree(buffr,&saveptr); 
    while(node != NULL){
        if(node != NULL && ! node -> is_atom){
#ifdef DEBUG
            print_prog(node);
#endif /* DEBUG */
            result = (int *)exec_prog(node,-1);
        }
        free_ast();
        node = build_tree(saveptr,&saveptr);
    }
}

