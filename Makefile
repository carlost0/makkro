CC = clang
CFLAGS = -g -Wall -Wextra  -DMAKKRO_PRINT_DEBUG_INFO -std=gnu99
src = src/lexer.c src/util.c
lib = src/x11*.c

all: lib lang

lib:
	${CC} ${CFLAGS} -c src/x11*.c
	ar -cvq x11.a *.o

lang:
	${CC} ${CFLAGS} -x c -DSV_IMPLEMENTATION -o sv.o -c src/sv.h
	${CC} ${CFLAGS} -x c -DDA_IMPLEMENTATION -o sv_da.o -c src/sv_da.h
	${CC} ${CFLAGS} -o lexer.o -c src/lexer.c
	${CC} ${CFLAGS} -o util.o -c src/util.c
	${CC} ${CFLAGS} -o parser.o -c src/parser.c
	${CC} ${CFLAGS} -v -DUSE_X11 -o makkro src/makkro.c sv.o sv_da.o parser.o lexer.o util.o x11.a -lX11 -lm
#   ${CC} ${CFLAGS} -DUSE_X11 -o makkro src/makkro.c parser.o lexer.o util.o x11.a -lX11 -lm


clean:
	-rm *.a
	-rm *.o
	-rm makkro

