#ifndef __LIBSMTP_H
#define __LIBSMTP_H
#define TAILLE 1000;
struct Courriel
{
	char adress_from[TAILLE];
	char adress_to[TAILLE];
    char *data;
};

void * gestion_SMTP();
#endif // !__LIBSMTP_H
