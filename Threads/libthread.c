#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "libthread.h"
#include <semaphore.h>

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

void mutex_init(void)
{
	int i ;
	for( i = 0; i< N; i++ )
		pthread_mutex_init(&mutex[i], NULL);
}

void P(int p)
{
	pthread_mutex_lock(&mutex[p]);
}

void v(int p)
{
	pthread_mutex_unlock(&mutex[p]);
}