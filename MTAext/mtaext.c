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
	printf("----------------------\n");

	MX *mxresolu = malloc(MAX_MX* sizeof(MX));
	//MX mxresolu;
	resolution_DNS(courriel->adress_from, mxresolu, MAX_MX);
	
	for(int i = 0; i< sizeof(mxresolu); i++)
	{
		printf(" prio = %d | mx resolu =  %s\n", mxresolu[i].prio, mxresolu[i].mx);

	}
	/* Connection au serveur */
	int s;
	for(int i = 0; i < MAX_MX; i++)
	{
		s=connexionServeur(mxresolu[0].mx,"25");
		if(s<0){ fprintf(stderr,"Erreur de connexion au serveur\n"); }

	}
	
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
