CC = cc
CFLAGS = -g -std=c89 -Wall -Wextra

clean:
	-rm *.a
	-rm *.o
	-rm makkro

lib:
	${CC} ${CFLAGS} -c src/x11*.c
	ar -cvq x11.a *.o

lang:
	${CC} ${CFLAGS} -o lexer.o -c src/lexer.c
	${CC} ${CFLAGS} -o util.o -c src/util.c -lm
	${CC} ${CFLAGS} -o parser.o -c src/parser.c 
	${CC} ${CFLAGS} -DUSE_X11 -o makkro src/makkro.c lexer.o util.o x11.a -lX11 -lm


