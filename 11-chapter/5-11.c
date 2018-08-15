#include "apue.h"
#include <pthread.h>

void cleanup(void *arg)
{
	printf("cleanup %s\n", (char *)arg);
}

void *thr_fn1(void *arg)
{
	char p1[] = "thread 1 first handler";
	char p2[] = "thread 1 second handler";
	printf("thread 1 statrt\n");
	pthread_cleanup_push(cleanup, p1);
	pthread_cleanup_push(cleanup, p2);
	printf("thread 1 push compelete\n");
	if(arg)
		return ((void *)1);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	return ((void *)1);
}

void *thr_fn2(void *arg)
{
	char p1[] = "thread 1 first handler";
	char p2[] = "thread 2 second handler";
	printf("thread 2 start\n");
	pthread_cleanup_push(cleanup, p1);
	pthread_cleanup_push(cleanup, p2);
	printf("thread 2 push complete\n");
	if(arg)
		pthread_exit((void *)2);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_exit((void *)2);
}

int main(void)
{
	int				err;
	pthread_t		tid1, tid2;
	void			*tret;

	err = pthread_create(&tid1, NULL, thr_fn1, ((void *)1));
	if(err != 0)
		err_exit(err, "cannot create thread 1");
	err = pthread_create(&tid2, NULL, thr_fn2, ((void * )1));
	if(err != 0)
		err_exit(err, "cannot create thread2");
	err = pthread_join(tid1, &tret);
	if(err != 0)
		err_exit(err, "cannot join with thread 1");
	printf("thread 1 exit code %ld\n", (long)tret);
	err = pthread_join(tid2, &tret);
	if(err != 0)
		err_exit(err, "cannot join with thrad 2");
	printf("thread 2 exit code %ld\n", (long)tret);
	exit(0);
}
