#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "libthread.h"


void lanceThread(void (*fonction)(void *), void *d, int taille)
{
	pthread_t tid;
	struct S *s = malloc(sizeof(struct S));
	s->arg = malloc(taille);
	memcpy(s->arg,d,taille);
	s->f = fonction;
	pthread_create(&tid,NULL,_lanceThread,s);
	pthread_detach(tid);
}


void * _lanceThread(void *arg)
{
	struct S *s_gc = arg;
	s_gc->f(s_gc->arg);
	free(s_gc->arg);
	free(arg);
	return NULL;
}