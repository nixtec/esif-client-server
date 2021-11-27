include makefile.head

INSTDIR        := usr/local/bin
REGDIR         := var/regform
PASSDIR        := $(REGDIR)/passwd
PASSDB         := $(PASSDIR)/pass.db
USER           := rajshahi
GROUP          := rajshahi


.PHONY: clean strip initialize

all: eduserver

eduserver: eduserver.o password.o regdb.o support.o
	$(CC) -o eduserver eduserver.o password.o regdb.o support.o $(LDFLAGS)

eduserver.o: eduserver.h eduserver.c password.h regdb.h support.o support.h
	$(CC) $(CFLAGS) -c eduserver.c

password.o: password.h password.c
	$(CC) $(CFLAGS) -c password.c

regdb.o: regdb.h regdb.c
	$(CC) $(CFLAGS) -c regdb.c

support.o: support.h support.c
	$(CC) $(CFLAGS) -c support.c

inspass: password.o inspass.c
	$(CC) $(CFLAGS) -o inspass inspass.c password.o $(LDFLAGS)

getpass: password.o getpass.c
	$(CC) $(CFLAGS) -o getpass getpass.c password.o $(LDFLAGS)

showpassdb: password.o showpassdb.c
	$(CC) $(CFLAGS) -o showpassdb showpassdb.c password.o $(LDFLAGS)

showregdb: regdb.o showregdb.c
	$(CC) $(CFLAGS) -o showregdb showregdb.c regdb.o $(LDFLAGS)

delpass: password.o delpass.c
	$(CC) $(CFLAGS) -o delpass delpass.c password.o $(LDFLAGS)

dispass: password.o dispass.c
	$(CC) $(CFLAGS) -o dispass dispass.c password.o $(LDFLAGS)

enapass: password.o enapass.c
	$(CC) $(CFLAGS) -o enapass enapass.c password.o $(LDFLAGS)

install: eduserver inspass delpass enapass dispass rc.eduserver showpassdb showregdb initialize
	cp -f eduserver inspass delpass enapass dispass showpassdb showregdb rc.eduserver $(INSTDIR)

genpass: genpass.o genpass.c
	$(CC) -o genpass genpass.c -O2 -Wall
initialize:
	mkdir -p $(REGDIR); \
	chown $(USER):$(GROUP) $(REGDIR); \
	mkdir -p $(PASSDIR); \
	chown $(USER):$(GROUP) $(PASSDIR); \
	if test -f $(PASSDB) ; then chmod 0700 $(PASSDB); fi

include makefile.tail
