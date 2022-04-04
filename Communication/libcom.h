#ifndef __LIBCOM_C__
#define __LIBCOM_C__

/** fichier libcom.h **/

/******************************************************************/
/** Ce fichier decrit les structures et les constantes utilisees **/
/** par les fonctions de gestion des sockets                     **/
/******************************************************************/

/**** Constantes ****/
#define MAX_LIGNE 1000
#define MAX_TAMPON 12000
#define MAX_MX 64

typedef struct MX
{
	char mx[100];
	int prio;
}MX;

/**** Fonctions ****/

int boucleServeur(int ecoute,void (*traitement)(int ));
int initialisationServeur(char *service,int connexions);
int connexionServeur(char *hote,char *service);
void resolution_DNS(char * hote);
int prioComparator ( const void  * first, const void * second );


#endif // !__LIBCOM_C__
