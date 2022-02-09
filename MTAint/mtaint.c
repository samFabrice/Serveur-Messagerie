#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <poll.h>
#include "mtaint.h"
#include <libsmtp.h>
#include <libcom.h>


int main(int argc,char *argv[])
{
	int s;

	/* Lecture des arguments de la commande */
	//analyseArguments(argc,argv);
	char *port = "2000";
	char *machine = "127.0.0.1";
	/* Connection au serveur */
	s=connexionServeur(machine,port);
	if(s<0){ fprintf(stderr,"Erreur de connexion au serveur\n"); exit(EXIT_FAILURE); }

	/* Boucle de communication avec le serveur */
	struct pollfd descripteurs[2];
	descripteurs[0].fd=s;
	descripteurs[0].events=POLLIN;
	descripteurs[1].fd=0;
	descripteurs[1].events=POLLIN;
	while(1){
  		char tampon[MAX_TAMPON];
  		int nb=poll(descripteurs,2,-1);
  		if(nb<0){ perror("main.poll"); exit(EXIT_FAILURE); }
  		if((descripteurs[0].revents&POLLIN)!=0){
    		int taille=read(s,tampon,MAX_TAMPON);
    		if(taille<=0) break;
    		write(1,tampon,taille);
    		}
  		if((descripteurs[1].revents&POLLIN)!=0){
    		int taille=read(0,tampon,MAX_TAMPON);
    		if(taille<=0) break;
    		write(s,tampon,taille);
    		}
  	}

	/* On termine la connexion */
	shutdown(s,SHUT_RDWR);

	return 0;
}
