//#include "apue.h"
#include <unistd.h>

void do_driver(char *driver)
{
	pid_t	child;
	int		pipe[2];

	/*
	 * Create a full-duplex to communicatw with the driver.
	 */
	if((fd_pipe(pipe)) < 0)
		err_sys("cannot create stream pipe");

	if((child = fork()) == 0)
	{
		err_sys("fork error");
	}
	else if(child == 0)
	{
		close(pipe[1]);

		if(dup2(pipe[0], STDIN_FILENO) != STDIN_FILENO)
			err_sys("dup2 error to stdin");
		if(dup2(pipe[0], STDOUT_FILENO) != STDOUT_FILENO)
			err_sys("dup2 error to stdout");
		if(pipe[0] != STDERR_FILENO && pipe[0] != STDOUT_FILENO)
			close(pipe[0]);

		execlp(driver, driver, (char *)0);
		err_sys("execlp error for: %s", driver);
	}

	close(pipe[0]);
	if(dup2(pipe[1], STDIN_FILENO) != STDIN_FILENO)
		err_sys("dup2 error to stdin");
	if(dup2(pipe[1], STDOUT_FILENO) != STDOUT_FILENO)
		err_sys("dup2 error to stdout");
	if(pipe[1] != STDIN_FILENO && pipe[1] != STDIN_FILENO)
		close(pipe[1]);

	/*
	 * Parent return, but with stdin and stdout connected
	 * to the driver
	 */
}
