#include "apue.h"
#include <errno.h>

void pr_mask(const char *str);
static void sig_int(int);

int main(void)
{
	sigset_t newmask, oldmask, waitmak;

	pr_mask("program start: ");

	if(signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal(SIGINT) == SIG_ERR");

	sigemptyset(&waitmak);
	sigaddset(&waitmak, SIGUSR1);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);

	if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		err_sys("SIG_BLOCK error");
	pr_mask("in critacal region: ");

	if(sigsuspend(&waitmak) != -1)
		err_sys("sigsuspend error");

	pr_mask("after return from sigsuspend: ");

	if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("SIG_SETMASK error");

	pr_mask("program exit: ");
	exit(0);
}
void pr_mask(const char* str)
{
        sigset_t sigset;
        int errno_save;
 
        errno_save = errno;
        if(sigprocmask(0,NULL,&sigset)<0)
        {
                printf("sigprocmask error\n");
                exit(-1);
        }
 
        printf("%s",str);
 
        if(sigismember(&sigset,SIGINT)) printf(" SIGINT");
 
        if(sigismember(&sigset,SIGQUIT)) printf(" SIGQUIT");
        if(sigismember(&sigset,SIGUSR1)) printf(" SIGUSR1");
        if(sigismember(&sigset,SIGALRM)) printf(" SIGALRM");
 
        printf("\n");
 
        errno = errno_save;
}

static void sig_int(int signo)
{
	pr_mask("\nin sigint: ");
}
