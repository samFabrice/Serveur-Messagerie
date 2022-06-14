#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <libsmtp.h>
#include <libthread.h>
#include <libcom.h>

// Initialisation serveur generique :
void gestionMAIL(Courriel *courriel)
{
	printf("MAIL FROM : %s\n", courriel->adress_from);
	printf("RCPT TO : %s\n", courriel->adress_to);
	printf("subject : %s\n", courriel->subject);
	printf("body : %s\n", courriel->body);
	printf("----------------------");
	resolution_DNS(courriel->adress_from);
}

void _gestionSMTP(void * arg)
{
	int d = *(int *)arg;
	gestionSMTP(d,gestionMAIL);
	
}
void gestionClient(int d)
{
	lanceThread(_gestionSMTP,&d,sizeof(int));
}

int main(int argc,char *argv[])
{
	int s;

	/* Lecture des arguments de la commande */
	//analyseArguments(argc,argv);
 	char *service = "4000";
	/* Initialisation du serveur */
	s=initialisationServeur(service,MAX_CONNEXIONS);

	/* Lancement de la boucle d'ecoute */
	boucleServeur(s,gestionClient);
}
