#ifndef __LIBSMTP_H
#define __LIBSMTP_H

/******************************************************************/
/** Ce fichier decrit les structures et les constantes utilisees **/
/** par les fonctions de gestion des sockets                     **/
/******************************************************************/

/**** Constantes ****/
#define MAX_LIGNE 1000
#define MAX_CONNEXIONS 1
#define MAX_COMMAND 100
#define TAILLE 1000

/**** Fonctions ****/

struct Courriel
{
	char adress_from[TAILLE];
	char adress_to[TAILLE];
    char *data;
};

void _gestionSMTP(int *d);
void gestionSMTP(void *s);

#endif // !__LIBSMTP_H
