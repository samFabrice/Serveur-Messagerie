#ifndef __LIBSMTP_H
#define __LIBSMTP_H

/******************************************************************/
/** Ce fichier decrit les structures et les constantes utilisees **/
/** par les fonctions de gestion des sockets                     **/
/******************************************************************/
#include <stdio.h>
#include <stdlib.h>
/**** Constantes ****/
#define MAX_LIGNE 1000
#define MAX_CONNEXIONS 1
#define MAX_COMMAND 100
#define TAILLE 1000

/**** Fonctions ****/

typedef struct Courriel
{
	char adress_from[TAILLE]; // l'expéditeur
	char adress_to[TAILLE]; // le destinataire
    char *body;             // coprs du message   
	char id[TAILLE]; //Pour la commande Hello
	char subject[TAILLE];
}Courriel;


void gestionSMTP(void *s);
void command_QUIT(FILE * fd,  Courriel * );
void command_HELO(char * command, Courriel * , FILE * fd);
void command_MAIL_FROM(char * command,  Courriel * , FILE * fd);
void command_RCPT_TO(char * command,  Courriel * , FILE * fd);
void command_DATA(struct Courriel * courriel, FILE * fd);




#endif // !__LIBSMTP_H
