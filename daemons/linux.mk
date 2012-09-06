include ../Make.defines.linux
EXTRA=
EXTRALIBS=-pthread

all:	init.o lockfile.o reread reread2 single.o

init.o:		init.c

lockfile.o:	lockfile.c

single.o:	single.c

reread: reread.o single.o lockfile.o init.o

reread2: reread2.o single.o lockfile.o init.o

clean:
	rm -f ${PROGS} ${TEMPFILES} *.o
