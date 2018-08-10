#include <unistd.h>
#include <signal.h>

static void sig_alarm(int signo)
{
	/* nothing to do, just return to wake up the pause */
}

unsigned int sleep1(unsigned int seconds)
{
	if(signal(SIGALRM, sig_alarm) == SIG_ERR)
		return (seconds);
	alarm(seconds);
	pause();
	return (alarm(0));
}
