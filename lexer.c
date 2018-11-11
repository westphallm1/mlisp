#include "keywords.h"
#include "mlisp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NOT_STARTED 999
#define IN_QUOTES 1
#define IS_ALPHA 2

/* */
int __get_alpha_token(char * stream, char ** saveptr){
    char to_hash[4] = {0,0,0,0};
    char * start_pos = stream;
    while(*stream >= 'a' && *stream <= 'z'){
        if(stream - start_pos < 4){
            to_hash[stream - start_pos] = *stream;
        }
        stream += 1;
    }
    *saveptr = stream;
    int token = CHR_CODE_LONG(to_hash[0],to_hash[1],to_hash[2],to_hash[3]);
    if(is_kw(token)){
        return CHR_CODE(to_hash[0],to_hash[1],to_hash[2],to_hash[3]);
    } else {
        return ID;
    }
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
        case '+':
        case '*':
        case '-':
        case '/':
            /* '<=', '>=', '+=', '-=', '*=', '/=' */
            next_good = next == '=';
            token += (next_good)?'='<<7:0;
            stream += (next_good)?1:0;
        case '=':
            /* '++', '--', '<<', '>>', and '==' */
            next_good = next == start;
            token += (next_good)?next<<7:0;
            stream += (next_good)?1:0;
            break;
        default:
            /* all single character tokens */
            break;
    }
    stream+=1;
    *saveptr = stream;
    return token%HASH_VAL;
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


/* read characters from a data stream until a complete statement is read */

int get_stmt(char * buffr, int max_len, FILE * stream){
    int paren_level = NOT_STARTED;    
    char parse_status = 0;
    char * curr = buffr;
    while(paren_level){
        if((*curr = fgetc(stream))==EOF)
            return EOF;
        /*check whether we're in a string literal, ignore special considerations
        if we are*/
        if(*curr == '"')
            parse_status ^= IN_QUOTES;

        if(!(parse_status & IN_QUOTES)){
            if(paren_level == NOT_STARTED && (*curr == '(' || *curr == ')'))
                paren_level = 0;
            if(*curr == '(')
                paren_level+=1;
            if(*curr == ')')
                paren_level-=1;
                /*only keep a single whitespace character between tokens*/
#if 0
            if(*curr == ' ' || *curr == '\n'){
                if(parse_status | HAS_WHITESPACE){
                    curr--;
                } else {
                    parse_status |= HAS_WHITESPACE;
                }
            }else{
                parse_status &= ~(HAS_WHITESPACE);
            }
#endif
        }
        curr++;
        if(curr - buffr > max_len){
            ERR("Input buffer too small for program.");
        }
    }
    return curr - buffr;
};
