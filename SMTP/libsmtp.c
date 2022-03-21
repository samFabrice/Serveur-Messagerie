#include <errno.h>
#include <string.h>
#include "libsmtp.h"
#include <time.h>

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
		sscanf(ligne,"%4s %[^\n]",command, buffer);
		if(strcmp(command,"HELO")==0)
		{
			command_HELO(buffer, &courriel, dialogue);
		}
		else if(strcmp(command, "QUIT")==0)
		{
			command_QUIT(dialogue,&courriel);
			break;
		}
		else if(strcmp(command,"MAIL") == 0)
		{
			char dummy[100];
			char arg[100];
            sscanf(ligne,"%s %5s %s",dummy, arg, buffer);
			if (strcmp(arg,"FROM:") == 0)
			{
				command_MAIL_FROM(buffer, &courriel, dialogue);

			}
			else
			{
				fprintf(dialogue, "501 - Error syntaxe\r\n");

			}
		}
		else if(strcmp(command,"RCPT") == 0)
		{
			char dummy[100];
			char arg[100];
            sscanf(ligne,"%s %5s %s",dummy, arg, buffer);
			if (strcmp(arg,"TO:") == 0)
				command_RCPT_TO(buffer, &courriel, dialogue);
			else
			{
				fprintf(dialogue, "501 - Error syntaxe\r\n");

			}
		}
		else if(strcmp(command, "DATA")==0)
		{
			command_DATA(&courriel, dialogue);

		}
		else
		{

			fprintf(dialogue, "501 - Error syntaxe\r\n");
		}
		memset(command, 0, MAX_COMMAND);
	}		

	/* Termine la connexion */

	fclose(dialogue);
}

void command_HELO(char * buffer, struct Courriel *courriel, FILE * fd)
{
	strcpy(courriel->id ,buffer);
	fprintf(fd,"250 - %s\r\n", courriel->id);

}

void command_QUIT( FILE * fd,struct Courriel *courriel)
{
	fprintf(fd, "221 - Bye %s\r\n",courriel->id);

}

void command_MAIL_FROM(char * buffer, struct Courriel *courriel, FILE * fd)
{
	strcpy(courriel->adress_from ,buffer);
	fprintf(fd,"250  OK, %s\r\n", courriel->adress_from);

}

void command_RCPT_TO(char * buffer, struct Courriel *courriel, FILE * fd)
{
	strcpy(courriel->adress_to ,buffer);
	fprintf(fd,"250  OK, %s\r\n", courriel->adress_to);

}

void command_DATA(struct Courriel * courriel, FILE * fd)
{
	time_t now;
	time(&now);
	char ligne[MAX_LIGNE];

	fprintf(fd,"354  END DATA with <CR><LF>.<CR><LF>\r\n");
	fprintf(fd, " DATE : %s\r\n",ctime(&now));
	fprintf(fd,"FROM : %s\r\n", courriel->adress_from);
	fprintf(fd,"TO : %s\r\n", courriel->adress_to);

	fprintf(fd, "Subject : ");

	// On écrit l'objet du mail
	if (fgets(ligne,MAX_LIGNE,fd) != NULL)
	{
		sscanf(ligne,"%s",courriel->subject);
	}
	//Lecture du corps du message
	int size_body;
	char buffer[MAX_LIGNE];
	size_body=sizeof(buffer);
	courriel->body = malloc(size_body);

	while(fgets(ligne,MAX_LIGNE,fd) != NULL)
	{
		sscanf(ligne,"%[^\n]",buffer);
		strcat(buffer, "\r\n");
		if(strcmp(buffer, ".\r\n") == 0)
			break;
		else {
			size_body += sizeof(buffer);
			courriel->body = realloc(courriel->body, size_body);
			strcat(courriel->body, buffer);
		}
	}

	fprintf(fd,"\r\n 250  OK, %s\r\n", courriel->body);
	free(courriel->body);
}
