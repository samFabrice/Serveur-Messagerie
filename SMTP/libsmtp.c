#include <errno.h>
#include <string.h>
#include "libsmtp.h"
#include <time.h>

void gestionSMTP(int fd, void (*dns)(Courriel *courriel)){

	/* Obtient une structure de fichier */
	FILE *dialogue=fdopen(fd,"a+");
	if(dialogue==NULL){ perror("gestionSMTP.fdopen"); exit(EXIT_FAILURE); }

	/* Echo */
	char ligne[MAX_LIGNE];
	char command[MAX_COMMAND];
	Courriel courriel;
	fprintf(dialogue,"220 Bienvenue SMTP\r\n");
	memset(courriel.adress_to, '\0', sizeof(courriel.adress_to));
	memset(courriel.adress_from, '\0', sizeof(courriel.adress_from));
	memset(courriel.id, '\0', sizeof(courriel.id));
	memset(courriel.subject, '\0', sizeof(courriel.subject));



	while(fgets(ligne,MAX_LIGNE,dialogue)!=NULL)
	{

		sscanf(ligne,"%4s",command);
		if(strcmp(command,"HELO")==0)
		{
			command_HELO(ligne, &courriel, dialogue);
		}
		else if(strcmp(command,"QUIT")==0)
		{
			command_QUIT(dialogue,&courriel);
			break;
		}
		else if(strcmp(command,"MAIL") == 0)
		{
			char dummy[100];
			char arg[100];
			int ret;
            ret = sscanf(ligne,"%s %5s",dummy, arg);
			if(ret == 2)
			{
				if (strcmp(arg,"FROM:") == 0)
					command_MAIL_FROM(ligne, &courriel, dialogue);
				else
					fprintf(dialogue, "501 - Error syntaxe\r\n");

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
			int ret;
            ret = sscanf(ligne,"%s %3s",dummy, arg);
			if(ret == 2)
			{
				if (strcmp(arg,"TO:") == 0)
				{
					command_RCPT_TO(ligne, &courriel, dialogue);
				}
				else
				{
					fprintf(dialogue, "501 - Error syntaxe\r\n");
				}
			}
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
	}

	dns(&courriel);
	/* Termine la connexion */

	fclose(dialogue);

}

void command_HELO(char * ligne, Courriel *courriel, FILE * fd)
{
	char buffer1[MAX_LIGNE];
	char buffer2[MAX_LIGNE];
	char buffer3[MAX_LIGNE];
	memset(buffer1, '\0',MAX_LIGNE);
	memset(buffer2, '\0',MAX_LIGNE);
	memset(buffer3, '\0',MAX_LIGNE);
	int ret;
	ret = sscanf(ligne, "%s %s %s", buffer1, buffer2, buffer3);
	if(ret ==2)
	{
		strcpy(courriel->id ,buffer2);
		fprintf(fd,"250 - %s\r\n", courriel->id);
	}
	else
		fprintf(fd,"501 - %s\r\n", courriel->id);


}

void command_QUIT( FILE * fd, Courriel *courriel)
{
	fprintf(fd, "221 - Bye %s\r\n",courriel->id);

}

void command_MAIL_FROM(char * ligne, Courriel *courriel, FILE * fd)
{
	char buffer1[MAX_LIGNE];
	char buffer2[MAX_LIGNE];
	char buffer3[MAX_LIGNE];
	memset(buffer1, '\0',MAX_LIGNE);
	memset(buffer2, '\0',MAX_LIGNE);
	memset(buffer3, '\0',MAX_LIGNE);
	int ret;
	ret = sscanf(ligne, "%s %s %s", buffer1, buffer2, buffer3);
	if(ret ==3)
	{
		strcpy(courriel->adress_from ,buffer3);
		fprintf(fd,"250 - %s\r\n",courriel->adress_from);
	}
	else
		fprintf(fd,"501  - Erreur nombre d'argument\r\n");

}

void command_RCPT_TO(char * ligne, Courriel *courriel, FILE * fd)
{
	char buffer1[MAX_LIGNE];
	char buffer2[MAX_LIGNE];
	char buffer3[MAX_LIGNE];
	memset(buffer1, '\0',MAX_LIGNE);
	memset(buffer2, '\0',MAX_LIGNE);
	memset(buffer3, '\0',MAX_LIGNE);
	int ret;
	ret = sscanf(ligne, "%s %s %s", buffer1, buffer2, buffer3);
	if(ret ==3)
	{
		strcpy(courriel->adress_to ,buffer3);
		fprintf(fd,"250 - %s\r\n",courriel->adress_to);
	}
	else
		fprintf(fd,"501  - Erreur nombre d'argument\r\n");

}

void command_DATA( Courriel * courriel, FILE * fd)
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
	memset(courriel->body, '\0', size_body);
	memset(buffer, '\0', sizeof(buffer));
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
