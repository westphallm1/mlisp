#include "keywords.h"
#include "mlisp.h"
#include <stdint.h>
#include <stdio.h>

/* */
int __get_alpha_token(char * stream, char ** saveptr){
    uint32_t token = 0;
    int lshift = 0;
    while(*stream >= 'a' && *stream <= 'z'){
        if(lshift < 32){
            token += ((uint32_t) *stream)<<lshift;
            lshift += 8;
        }
        stream += 1;
    }
    *saveptr = stream;
    return token;
}

int __get_strlit_token(char * stream, char ** saveptr){
    stream += 1;
    while(*stream > 0 && *stream != '"'){
        stream += 1;
    }
    if(*stream == 0){
        /* unterminated string literal */
        return -1;
    } else {
        stream += 1;
        *saveptr = stream;
        return STRLIT;
    }
}

int __get_numeric_token(char * stream, char **saveptr){
    char is_float = 0;
    while(*stream=='.' || (*stream >='0' && *stream <= '9')){
        is_float |= *stream=='.';
        stream+=1;
    }
    *saveptr = stream;
    return (is_float)?FLOAT:INT;
}

/*
 * The token for a character is the uint32 consisting of the sum of its first 
 * 4 (or fewer) ascii characters
 */
int get_token_r(char * stream, char ** saveptr){
    
    char start;
    char next;
    char next_good;
    uint32_t token;

    if(stream == NULL){
        stream = *saveptr;
    }
    /* eat whitespace */
    if(*stream == 0)
        return 0;
    while(*stream <= ' '){
        stream+=1;
    }
    start = *stream;
    next = *(stream + 1);
    token = (uint32_t) start;
    
    if(start == '"'){
        /* string literals */
        return __get_strlit_token(stream,saveptr);
    }
    if(start == '.' || (start >='0' && start <='9')){
        /* numeric literals  */
        return __get_numeric_token(stream,saveptr);
    }
    if(start >= 'a' && start <= 'z'){
        /* all alphabetic tokens */
        return __get_alpha_token(stream,saveptr);
    }


    /* one and two character tokens */
    switch (start) {
        case '<':
        case '>':
            /* '<=' and '>=' */
            next_good = next == '=';
            token += (next_good)?'='<<8:0;
            stream += (next_good)?1:0;
        case '+':
        case '-':
        case '=':
            /* '++', '--', '<<', '>>', and '==' */
            next_good = next == start;
            token += (next_good)?next<<8:0;
            stream += (next_good)?1:0;
            break;
        default:
            /* all single character tokens */
            break;
    }
    stream+=1;
    *saveptr = stream;
    return token;
}

/*
 * Get the next token, then reset the stream head to its previous position
 * Not wildly efficient
 */
int peek_token_r(char * stream, char ** saveptr){
    char * head = *saveptr;
    int result = get_token_r(stream,saveptr);
    *saveptr = head;
    return result;
}
