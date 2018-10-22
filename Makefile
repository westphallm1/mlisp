

interpreter: interpreter.o lexer.o parser.o
	gcc $(CFLAGS) -o interpreter interpreter.o parser.o lexer.o

interpreter.o: interpreter.c mlisp.h keywords.h
	gcc $(CFLAGS) -c interpreter.c
parser.o: parser.c mlisp.h keywords.h
	gcc $(CFLAGS) -c parser.c
lexer.o: lexer.c mlisp.h keywords.h
	gcc $(CFLAGS) -c lexer.c
