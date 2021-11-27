all: goodclient

.PHONY: all clean

CFLAGS         := `pkg-config --cflags gtk+-2.0` -Wall -O2
LIBS           := `pkg-config --libs gtk+-2.0` -lcrypto

goodclient: goodclient.o myfuncs.o
	gcc -o goodclient goodclient.o myfuncs.o $(LIBS)


myfuncs.o: myfuncs.c myfuncs.h defines.h includes.h
	gcc -c myfuncs.c $(CFLAGS)

goodclient.o: goodclient.c
	gcc -c goodclient.c $(CFLAGS)

clean:
	-rm -f *.o
