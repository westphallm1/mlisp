#ifndef KEYWORDS_H
#define KEYWORDS_H
#define CHR_CODE(a,b,c,d) (a + (b<<7) + (c<<14) + (d<<21)) % 207

#define LPAREN CHR_CODE('(',0,0,0) // 37
#define RPAREN CHR_CODE(')',0,0,0) // 38
#define TRUE CHR_CODE('t','r','u','e') // 135
#define FALSE CHR_CODE('f','a','l','s') // 19
#define NIL CHR_CODE('n','i','l',0) // 155
#define IF CHR_CODE('i','f',0,0) // 183
#define ELSE CHR_CODE('e','l','s','e') // 118
#define WHILE CHR_CODE('w','h','i','l') // 180
#define DEF CHR_CODE('d','e','f',0) // 68
#define RET CHR_CODE('r','e','t',0) // 102
#define ARGS CHR_CODE('a','r','g','s') // 172
#define CALL CHR_CODE('c','a','l','l') // 185
#define ASSIGN CHR_CODE('=',0,0,0) // 58
#define PLUSASSIGN CHR_CODE('+','=',0,0) // 48
#define TIMESASSIGN CHR_CODE('*','=',0,0) // 47
#define MINUSASSIGN CHR_CODE('-','=',0,0) // 50
#define DIVASSIGN CHR_CODE('/','=',0,0) // 52
#define EQUALS CHR_CODE('=','=',0,0) // 66
#define LESS CHR_CODE('<',0,0,0) // 57
#define GREATER CHR_CODE('>',0,0,0) // 59
#define LESSEQ CHR_CODE('<','=',0,0) // 65
#define GREATEREQ CHR_CODE('>','=',0,0) // 67
#define AND CHR_CODE('&',0,0,0) // 35
#define OR CHR_CODE('|',0,0,0) // 121
#define NOT CHR_CODE('~',0,0,0) // 123
#define NEGATE CHR_CODE('!',0,0,0) // 30
#define PLUS CHR_CODE('+',0,0,0) // 40
#define INC CHR_CODE('+','+',0,0) // 21
#define MINUS CHR_CODE('-',0,0,0) // 42
#define DEC CHR_CODE('-','-',0,0) // 72
#define LSHIFT CHR_CODE('<','<',0,0) // 144
#define RSHIFT CHR_CODE('>','>',0,0) // 195
#define TIMES CHR_CODE('*',0,0,0) // 39
#define MOD CHR_CODE('%',0,0,0) // 34
#define DIVIDE CHR_CODE('/',0,0,0) // 44
#define SSTART CHR_CODE('s','s','t','a') // 91
#define SWRITE CHR_CODE('s','w','r','i') // 200
#define SREAD CHR_CODE('s','r','e','a') // 119
#define GPMODE CHR_CODE('g','p','m','o') // 175
#define GPSET CHR_CODE('g','p','s','e') // 11
#define GPREAD CHR_CODE('g','p','r','e') // 187
#define SLEEP CHR_CODE('s','l','e','e') // 112
#define ID CHR_CODE('i','d',0,0) // 134
#define STRLIT CHR_CODE('s','t','r','l') // 128
#define INT CHR_CODE('i','n','t',0) // 3
#define FLOAT CHR_CODE('f','l','o','a') // 62
#define ARGV CHR_CODE('a','r','g','v') // 70
#define ARGC CHR_CODE('a','r','g','c') // 26
#endif /* KEYWORDS_H */
