

interpreter: interpreter.o lexer.o parser.o is_kw.o mem_manage.o
	gcc $(CFLAGS) -o interpreter interpreter.o parser.o lexer.o is_kw.o \
		mem_manage.o

interpreter.o: interpreter.c slimp.h keywords.h config.h
	gcc $(CFLAGS) -c interpreter.c
parser.o: parser.c slimp.h keywords.h config.h
	gcc $(CFLAGS) -c parser.c
lexer.o: lexer.c slimp.h keywords.h config.h
	gcc $(CFLAGS) -c lexer.c

mem_manage.o: mem_manage.c slimp.h keywords.h config.h
	gcc $(CFLAGS) -c mem_manage.c

is_kw.o: is_kw.c keywords.h config.h
	gcc $(CFLAGS) -c is_kw.c

keywords.h: keywords.txt
	python3 gen_kw.py keywords.txt keywords.h is_kw.c

clean:
	rm -f  keywords.h is_kw.c *.o interpreter parser lexer
