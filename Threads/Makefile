#
# Makefile pour generer la bibliotheque de threads
#
all: libthread.a

#
# La cible de nettoyage
#

libthread.o : libthread.c libthread.h

clean:
	rm -rf core *.o libthread.a

libthread.a: libthread.o
	rm -f $@
	$(CLIB) $@ $+

