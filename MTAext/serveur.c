#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include "serveur.h"

// Initialisation serveur generique :


void _gestionClient(int *d)
{
	lanceThread(gestionClient,d,sizeof(int));
}

void gestionClient(void *s){

	/* Obtient une structure de fichier */
	int fd = *(int *) s;
	FILE *dialogue=fdopen(fd,"a+");
	if(dialogue==NULL){ perror("gestionClient.fdopen"); exit(EXIT_FAILURE); }

	/* Echo */
	char ligne[MAX_LIGNE];
	char command[MAX_COMMAND];
	fprintf(dialogue,"220 Bienvenue SMTP\r\n");
	while(fgets(ligne,MAX_LIGNE,dialogue)!=NULL)
	{
		//fprintf(dialogue, "> %s",ligne);
		sscanf(ligne,"%4s",command);
		if(strcmp(command,"HELO")==0)
			fprintf(dialogue,"250\r\n");
		
		else if(strcmp(command, "QUIT")==0)
		{
			 fprintf(dialogue, "221 Bye\r\n");
		}
		else
		{
			fprintf(dialogue, "501 Error synaxte\r\n");
		}
		
	}		

	/* Termine la connexion */

	fclose(dialogue);
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
	boucleServeur(s,_gestionClient);
} 
