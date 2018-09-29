//#include "apue.h"
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define BUFFSIZE	512

static void	sig_term(int);
static volatile sig_atomic_t	sigcaught;

ssize_t writen (int fd, const void *buf, size_t num)
{
ssize_t res;
size_t n;
const char *ptr;
 
 
n = num;
ptr = buf;
while (n > 0) {
/* 开始写*/ 
     if ((res = write (fd, ptr, n)) <= 0) {
      if (errno == EINTR)
       res = 0;
      else
       return (-1);
     }
 
 
     ptr += res;/* 从剩下的地方继续写     */ 
     n -= res;
}
 
 
return (num);
}

void loop(int ptym, int ignoreeof)
{
	pid_t	child;
	int		nread;
	char	buf[BUFFSIZE];

	if((child = fork()) < 0)
	{
		err_sys("fork error");
	}
	else if(child == 0)
	{
		/* child copies stdin to ptym */
		for(;;)
		{
			if((nread = read(STDIN_FILENO, buf, BUFFSIZE)) < 0)
				err_sys("read error from stdin");
			else if(nread == 0)
				break;	/* EOF on stdin means we're done */
			if(writen(ptym, buf, nread) != nread)
				err_sys("writen error to master pty");
		}

		if(ignoreeof == 0)
			kill(getppid(), SIGTERM);
		exit(0);
	}

	if(signal_intr(SIGTERM, sig_term) == SIG_ERR)
		err_sys("signal_intr error for SIGTERM");

	for(;;)
	{
		if((nread = read(ptym, buf, BUFFSIZE)) <= 0)
			break;
		if(writen(STDOUT_FILENO, buf, nread) != nread)
			err_sys("writen error to stdout");
	}
	if(sigcaught == 0)
		kill(child, SIGTERM);

}
/*
 * Parent returns to caller.
 */
static void sig_term(int signo)
{
	sigcaught = 1;
}
