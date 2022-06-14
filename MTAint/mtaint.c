#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <poll.h>
#include <pthread.h>
#include <libthread.h>
#include "mtaint.h"
#include <libsmtp.h>
#include <libcom.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

const char *home = "/HOME/";
const char *maildir = "/Maildir/";
const char *new = "/new/";
const char *tmp = "/tmp/";

void maildircheck(int ret){
    errno = 0;
    if (ret == -1) {
        switch (errno) {
            case EACCES :
                printf("the parent directory does not allow write");
                exit(EXIT_FAILURE);
            case EEXIST:
                printf("pathname already exists");
                exit(EXIT_FAILURE);
            case ENAMETOOLONG:
                printf("pathname is too long");
                exit(EXIT_FAILURE);
            default:
                perror("mkdir");
                exit(EXIT_FAILURE);
        }
    }

}
// Initialisation serveur generique :
void gestionMAIL(Courriel *courriel)
{
    char cwd[1024];
    chdir("");
    getcwd(cwd, sizeof(cwd));
    strcat(cwd, home);
    char *user = strtok(courriel->adress_to, "@");
    strcat(cwd,user);
    
    printf("-------> %s\n", cwd);
    int ret = mkdir(cwd,S_IRWXU);
    printf("-------> ret = %d\n", ret);
    maildircheck(ret);

    strcat(cwd, maildir);
    ret = mkdir(cwd,S_IRWXU);
    maildircheck(ret);

    char pwd[1024];
    memset(pwd, '\0', 1024);
    strcpy(pwd, cwd);

    strcat(cwd, new);
    ret = mkdir(cwd,S_IRWXU);
    maildircheck(ret);

    strcat(pwd, tmp);
    ret = mkdir(pwd,S_IRWXU);
    maildircheck(ret);

    char nomFichier[1024];
    memset(nomFichier, '\0', 1024);
    strcpy(nomFichier, pwd);
    strcat(nomFichier, "test.txt");

    FILE *fichier = NULL;
    fichier = fopen(nomFichier, "w");
    if (fichier != NULL)
    {
    printf("copie du corps du mail\n");
    fputs(courriel->body, fichier);
    fclose(fichier);
    }
    else 
        printf("************\n");

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