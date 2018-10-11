#include "keywords.h"
#include <stdint.h>

char * CURR_SYM;
int SYM_LENGTH;
/* */
int get_alpha_token(char ** stream){
    uint32_t token = 0;
    int lshift = 0;
    while(**stream >= 'a' && **stream <= 'z'){
        if(lshift < 32){
            token += ((uint32_t) **stream)<<lshift;
            lshift += 8;
        }
        *stream += 1;
    }
    SYM_LENGTH = *stream - CURR_SYM;
    return token;
}

int get_strlit_token(char ** stream){
    *stream += 1;
    while(**stream > 0 && **stream != '"'){
        *stream += 1;
    }
    if(**stream == 0){
        /* unterminated string literal */
        return -1;
    } else {
        *stream += 1;
        SYM_LENGTH = *stream - CURR_SYM;
        return STRLIT;
    }
}

/*
 * The token for a character is the uint32 consisting of the sum of its first 
 * 4 (or fewer) ascii characters
 */
int get_token(char ** stream){
    
    char start;
    char next;
    char next_good;
    uint32_t token;

    /* eat whitespace */
    if(**stream == 0)
        return 0;
    while(**stream <= ' '){
        *stream+=1;
    }
    CURR_SYM = *stream;
    start = **stream;
    next = *(*stream + 1);
    token = (uint32_t) start;
    
    if(start == '"'){
        /* string literals */
        return get_strlit_token(stream);
    }
    if(start >= 'a' && start <= 'z'){
        /* all alphabetic tokens */
        return get_alpha_token(stream);
    }

    /* one and two character tokens */
    switch (start) {
        case '<':
        case '>':
            /* '<=' and '>=' */
            next_good = next == '=';
            token += (next_good)?'='<<8:0;
            *stream += (next_good)?1:0;
        case '+':
        case '-':
        case '=':
            /* '++', '--', '<<', '>>', and '==' */
            next_good = next == start;
            token += (next_good)?next<<8:0;
            *stream += (next_good)?1:0;
            break;
        default:
            /* all single character tokens */
            break;
    }
    *stream+=1;
    SYM_LENGTH = *stream - CURR_SYM;
    return token;
}

/*
 * Get the next token, then reset the stream head to its previous position
 * Not wildly efficient
 */
int peek_token(char ** stream){
    int result = get_token(stream);
    *stream = CURR_SYM;
    return result;
}

int main(){
    char * lex_me = "(! hello \"hello world\" world)";
    char * curr = lex_me;
    uint32_t token;
    while(token = get_token(&curr)){
    }
}
