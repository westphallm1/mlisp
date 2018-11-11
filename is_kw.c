
#include "keywords.h"
int is_kw(int token){
    switch(token){
    case LPAREN_LONG:
    case RPAREN_LONG:
    case IF_LONG:
    case ELSE_LONG:
    case WHILE_LONG:
    case ASSIGN_LONG:
    case PLUSASSIGN_LONG:
    case TIMESASSIGN_LONG:
    case MINUSASSIGN_LONG:
    case DIVASSIGN_LONG:
    case EQUALS_LONG:
    case LESS_LONG:
    case GREATER_LONG:
    case LESSEQ_LONG:
    case GREATEREQ_LONG:
    case AND_LONG:
    case OR_LONG:
    case NOT_LONG:
    case NEGATE_LONG:
    case PLUS_LONG:
    case MINUS_LONG:
    case LSHIFT_LONG:
    case RSHIFT_LONG:
    case TIMES_LONG:
    case MOD_LONG:
    case DIVIDE_LONG:
    case SWRITE_LONG:
    case SREAD_LONG:
    case GPMODE_LONG:
    case GPSET_LONG:
    case GPREAD_LONG:
    case SLEEP_LONG:
    case ID_LONG:
    case STRLIT_LONG:
    case INT_LONG:
    case FLOAT_LONG:
    case ARGV_LONG:
    case ARGC_LONG:
    case TOINT_LONG:
    case STRCMP_LONG:
    case EXIT_LONG:
        return 1;
    }
    return 0;
}

