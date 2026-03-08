CC = cc

clean:
	-rm *.a
	-rm *.o
	-rm makkro

libX11:
	${CC} -Wall -c lib/x11/*.c
	ar -cvq x11_io.a *.o

x11IO:
	${CC} -DUSE_X11 -o makkro src/main.c x11_io.a -lX11

allX11: libX11 x11IO
