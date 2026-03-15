CC = cc
CFLAGS = -std=c89 -Wall -Wextra

clean:
	-rm *.a
	-rm *.o
	-rm makkro

lib:
	${CC} ${CFLAGS} -c src/x11*.c
	ar -cvq x11.a *.o

lang:
	${CC} ${CFLAGS} -DUSE_X11 -o makkro src/interpreter.c x11_io.a -lX11


