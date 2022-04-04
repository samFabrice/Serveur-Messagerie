#ifndef __libThread_H__
#define __libThread_H__

struct S
{
	int *arg;
	void (*f)(void *);
};

void * _lanceThread(void *arg);
void lanceThread(void (*fonction)(void *), void *d, int taille);
#endif