#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "libsmtp.h"


void gestionSMTP(void *s){

	/* Obtient une structure de fichier */
	int fd = *(int *) s;
	FILE *dialogue=fdopen(fd,"a+");
	if(dialogue==NULL){ perror("gestionSMTP.fdopen"); exit(EXIT_FAILURE); }

	/* Echo */
	char ligne[MAX_LIGNE];
	char command[MAX_COMMAND];
	char buffer[100];
	struct Courriel courriel;
	fprintf(dialogue,"220 Bienvenue SMTP\r\n");

	while(fgets(ligne,MAX_LIGNE,dialogue)!=NULL)
	{

		//fprintf(dialogue, "> %s",ligne);
		sscanf(ligne,"%4s %s",command, buffer);
		if(strcmp(command,"HELO")==0)
		{
			command_HELO(buffer, &courriel, dialogue);
		}

		else if(strcmp(command, "QUIT")==0)
		{
			 fprintf(dialogue, "221 Bye\r\n");
		}
		else if(strcmp(command,"MAIL FROM:") == 0)
		{

		}
		else if(strcmp(command,"RCPT TO:") == 0)
		{

		}
		else if(strcmp(command, "DATA")==0)
		{

		}
		else
		{
			fprintf(dialogue, "501 Error synaxte\r\n");
		}
		
	}		

	/* Termine la connexion */

	fclose(dialogue);
}

void command_HELO(char * buffer, struct Courriel * courriel, FILE * fd)
{
	fprintf(fd,"250 - %s\r\n", buffer);
	strcpy(courriel->id ,buffer);
}