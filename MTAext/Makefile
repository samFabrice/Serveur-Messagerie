#
# Makefile pour le serveur SMTP entrant
#

SOURCES = $(wildcard *.c)
OBJETS = $(SOURCES:.c=.o)
BIBLIOTHEQUES = -lthread -lcom -lsmtp -L ../Threads -L ../Communication -L ../SMTP -lpthread
CFLAGS += -std=c99 -I ../Threads -I ../Communication -I ../SMTP
CIBLE = smtp_ext

#
# Nom de la cible principale
#

all: $(CIBLE)

#
# Cible de nettoyage
#

clean: 
	rm -f core *.o $(CIBLE)

#
# Dependances pour l'executable
#

$(CIBLE): $(OBJETS)
	$(CC) -o $@ $^ $(BIBLIOTHEQUES)
