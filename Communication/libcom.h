#ifndef __LIBCOM_C__
#define __LIBCOM_C__

/** fichier libcom.h **/

/******************************************************************/
/** Ce fichier decrit les structures et les constantes utilisees **/
/** par les fonctions de gestion des sockets                     **/
/******************************************************************/

/**** Constantes ****/
#define MAX_LIGNE 1000

/**** Fonctions ****/
int boucleServeur(int ecoute,void (*traitement)(int *));
int initialisationServeur(char *service,int connexions);



#endif // !__LIBCOM_C__