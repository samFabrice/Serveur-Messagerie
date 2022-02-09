#
# Makefile pour generer la bibliotheque de fonctions de communication
#

all: libsmtp.a

#
# La cible de nettoyage
#

clean: 
	rm -f core *.o libsmtp.a

#
# Les cibles pour la bibliotheque
#

libsmpt.o: libsmtp.c libsmtp.h
 
libsmtp.a: libsmtp.o
	rm -f $@
	$(CLIB) $@ $+
