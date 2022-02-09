#ifndef __SERVEUR_H__
#define __SERVEUR_H__


/******************************************************************/
/** Ce fichier decrit les structures et les constantes utilisees **/
/** par les fonctions de gestion des sockets                     **/
/******************************************************************/

/**** Constantes ****/
#define MAX_LIGNE 1000
#define MAX_CONNEXIONS 1
#define MAX_COMMAND 100


/**** Fonctions ****/
struct S
{
	int *arg;
	void (*f)(void *);
};

void _gestionClient(int *d)
void gestionClient(void *s)


#endif // !__SERVEUR_H__