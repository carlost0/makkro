CC = cc
CFLAGS = -std=c89 -Wall -Wextra

clean:
	-rm *.a
	-rm *.o
	-rm makkro

x11:
	${CC} ${CFLAGS} -c src/x11*.c
	ar -cvq x11_io.a *.o
	${CC} ${CFLAGS} -DUSE_X11 -o makkro src/main.c x11_io.a -lX11
