

interpreter: interpreter.o lexer.o parser.o is_kw.o
	gcc $(CFLAGS) -o interpreter interpreter.o parser.o lexer.o is_kw.o

interpreter.o: interpreter.c mlisp.h keywords.h config.h
	gcc $(CFLAGS) -c interpreter.c
parser.o: parser.c mlisp.h keywords.h config.h
	gcc $(CFLAGS) -c parser.c
lexer.o: lexer.c mlisp.h keywords.h config.h
	gcc $(CFLAGS) -c lexer.c
is_kw.o: is_kw.c keywords.h config.h
	gcc $(CFLAGS) -c is_kw.c

keywords.h: keywords.txt
	python3 gen_kw.py keywords.txt keywords.h is_kw.c

clean:
	rm -f  keywords.h is_kw.c *.o interpreter parser lexer
