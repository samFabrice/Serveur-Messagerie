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
	// Envoie mail
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
