#include "apue.h"
#include <sys/wait.h>

char *env_init[] = {
	"USER=unknow",
	"PATH=/home/free/github/apue/8-chapter",
	NULL
};
int main(void)
{
	pid_t pid;

	if((pid = fork()) < 0)
	{
		err_sys("fork error");
	}
	else if(pid == 0)
	{
		if(execle("/home/free/github/apue/8-chapter/17-8", "17-8", "argv[1] test", "argv[2] test", (char *)0, env_init) < 0)
			err_sys("execle error");
	}

	if(waitpid(pid, NULL, 0) < 0)
		err_sys("wait error");

	if((pid = fork()) < 0)
	{
		err_sys("fork error");
	}
	else if(pid == 0)
	{
		if(execlp("17-8", "17-8", "just one argv", (char *)0) < 0)
			err_sys("execlp error");
	}

	exit(0);
}
