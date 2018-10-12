
parser: lexer.o parser.o
	gcc $(CFLAGS) -o parser parser.o lexer.o

parser.o: parser.c mlisp.h keywords.h
	gcc $(CFLAGS) -c parser.c
lexer.o: lexer.c mlisp.h keywords.h
	gcc $(CFLAGS) -c lexer.c
