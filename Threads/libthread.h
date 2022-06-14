#ifndef __libThread_H__
#define __libThread_H__
#define N 10
struct S
{
	int *arg;
	void (*f)(void *);
};

pthread_mutex_t mutex[N];
void * _lanceThread(void *arg);
void lanceThread(void (*fonction)(void *), void *d, int taille);
#endif