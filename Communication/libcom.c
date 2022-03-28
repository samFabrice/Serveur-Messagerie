/** fichier libcom.c **/

/*****************************************************************/
/** Ce fichier contient des fonctions concernant les sockets.   **/
/*****************************************************************/

/**** Fichiers d'inclusion ****/

#include <bits/types/res_state.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>

#include <errno.h>
#include <netdb.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <resolv.h>
#include "libcom.h"

/**** Constantes ****/

/**** Variables globales *****/

/**** Fonctions de gestion des sockets ****/

int initialisationServeur(char *service,int connexions){
	struct addrinfo precisions,*resultat,*origine;
	int statut;
	int s;

	/* Construction de la structure adresse */
	memset(&precisions,0,sizeof precisions);
	precisions.ai_family=AF_UNSPEC;
	precisions.ai_socktype=SOCK_STREAM;
	precisions.ai_flags=AI_PASSIVE;
	statut=getaddrinfo(NULL,service,&precisions,&origine);

	if(statut<0){ perror("initialisationServeur.getaddrinfo"); exit(EXIT_FAILURE); }
	struct addrinfo *p;
	for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
  		if(p->ai_family==AF_INET6){ resultat=p; break; }

	/* Creation d'une socket */
	s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
	if(s<0){ perror("initialisationServeur.socket"); exit(EXIT_FAILURE); }

	/* Options utiles */
	int vrai=1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&vrai,sizeof(vrai))<0){
  		perror("initialisationServeur.setsockopt (REUSEADDR)");
 		 exit(EXIT_FAILURE);
 	 }

	if(setsockopt(s,IPPROTO_TCP,TCP_NODELAY,&vrai,sizeof(vrai))<0){
  		perror("initialisationServeur.setsockopt (NODELAY)");
  		exit(EXIT_FAILURE);
  	}

	/* Specification de l'adresse de la socket */
	statut=bind(s,resultat->ai_addr,resultat->ai_addrlen);
	if(statut<0) return -1;

	/* Liberation de la structure d'informations */
	freeaddrinfo(origine);

	/* Taille de la queue d'attente */
	statut=listen(s,connexions);
	if(statut<0) return -1;

	return s;
}

int boucleServeur(int ecoute,void (*traitement)(int *))
{
    int dialogue;
	struct sockaddr adressClient;
	socklen_t adressClientLong = sizeof( adressClient);

	while(1){

    		/* Attente d'une connexion */
    		if((dialogue=accept(ecoute,&adressClient,&adressClientLong))<0)
			{
				perror("Failed to accept client socket");
				exit(EXIT_FAILURE);
			}
    		/* Passage de la socket de dialogue a la fonction de traitement */ 
			char ligne[MAX_LIGNE];
			traitement(&dialogue) ;

			if(recv(dialogue, ligne, sizeof(ligne), 0) == -1)
			{
				perror("Failed to receive message");
				exit(EXIT_FAILURE);
			}


    	}


}


int connexionServeur(char *hote,char *service){
	struct addrinfo precisions,*resultat,*origine;
	int statut;
	int s;

	/* Creation de l'adresse de socket */
	memset(&precisions,0,sizeof precisions);
	precisions.ai_family=AF_UNSPEC;
	precisions.ai_socktype=SOCK_STREAM;
	statut=getaddrinfo(hote,service,&precisions,&origine);
	if(statut<0){ perror("connexionServeur.getaddrinfo"); exit(EXIT_FAILURE); }
	struct addrinfo *p;
	for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
  	if(p->ai_family==AF_INET6){ resultat=p; break; }

	/* Creation d'une socket */
	s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
	if(s<0){ perror("connexionServeur.socket"); exit(EXIT_FAILURE); }

	/* Connection de la socket a l'hote */
	if(connect(s,resultat->ai_addr,resultat->ai_addrlen)<0) return -1;

	/* Liberation de la structure d'informations */
	freeaddrinfo(origine);

	return s;
}

void  resolution_DNS(char *hote)
{
	struct __res_state res;
	res_ninit(&res);

	u_char answer[NS_PACKETSZ];
	int len = res_nquery(&res,"gmail.com", C_IN,T_MX, answer, sizeof(answer));
	if(len == 0) exit(-1);
	printf("len = %d", len);
	ns_msg handle ;
	int status = ns_initparse(answer, len, &handle);
	if(status) exit(-2);

	int count, ret, type; 
	uint16_t prio;
	const unsigned char *rdata;
	char buffer[2000];
	count = ns_msg_count(handle, ns_s_an);
	printf("%d\n", count);
	ns_rr rr;

	for(int i =0; i < count; i++)
	{
		ret = ns_parserr(&handle, ns_s_an, i, &rr);

		if(ret) break;
		type =  ns_rr_type(rr);
		printf("type = %d\n", type);
		if(type == ns_t_mx)
		{

			rdata = ns_rr_rdata(rr);
			NS_GET16(prio, rdata);
			dn_expand(ns_msg_base(handle), ns_msg_end(handle), rdata, buffer, sizeof(buffer) );
			printf("%d %s\n", prio, buffer);
		}


	}
}