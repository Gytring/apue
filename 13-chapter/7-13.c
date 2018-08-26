#include "apue.h"
#include <pthread.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

sigset_t mask;

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

int lockfile(int fd)
{
	struct flock fl;

	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;
	return(fcntl(fd, F_SETLK, &fl));
}

int already_running(void)
{
	int		fd;
	char	buf[16];

	fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
	if(fd < 0)
	{
		syslog(LOG_ERR, "cannot open %s: %s", LOCKFILE, strerror(errno));
		exit(1);
	}
	if(lockfile(fd) < 0)
	{
		if(errno == EACCES || errno == EAGAIN)
		{
			close(fd);
			return(1);
		}
		syslog(LOG_ERR, "cannot lock %s: %s", LOCKFILE, strerror(errno));
		exit(1);
	}
	ftruncate(fd, 0);
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf)+1);
	return(0);
}

void daemonize(const char *cmd)
{
	int					i, fd0, fd1, fd2;
	pid_t				pid;
	struct rlimit		r1;
	struct sigaction	sa;

	umask(0);
	if(getrlimit(RLIMIT_NOFILE, &r1) < 0)
		err_quit("%s: cannot get file limit", cmd);

	if((pid = fork()) < 0)
		err_quit("%s: cannot fork", cmd);
	else if(pid != 0)
		exit(0);
	setsid();

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if(sigaction(SIGHUP, &sa, NULL))
		err_quit("%s: cannot ignore SIGHUP", cmd);
	if((pid = fork()) < 0)
		err_quit("%s: cannot fork", cmd);
	else if(pid != 0)
		exit(0);

	if(chdir("/") < 0)
		err_quit("%s: cannot cahnge diretory to /", cmd);

	if(r1.rlim_max == RLIM_INFINITY)
		r1.rlim_max = 1024;
	for(i = 0; i < r1.rlim_max; i++)
		close(i);

	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if(fd0 != 0 || fd1 != 1 || fd2 != 2)
	{
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
		exit(1);
	}
}

void reread(void)
{
	/****/
}

void *thr_fn(void *arg)
{
	int err, signo;

	for(;;)
	{
		err = sigwait(&mask, &signo);
		if(err != 0)
		{
			syslog(LOG_ERR, "sigwait failed");
			exit(1);
		}
		switch(signo)
		{
			case SIGHUP:
				syslog(LOG_INFO, "Re-reading configuration file");
				reread();
				break;

			case SIGTERM:
				syslog(LOG_INFO, "got SIGTERM exiting");
				exit(0);

			default:
				syslog(LOG_INFO, "unexpected signal %d\n", signo);
		}
	}
	return(void *)0;
}

int main(int argc, char *argv[])
{
	int					err;
	pthread_t			tid;
	char				*cmd;
	struct sigaction	sa;

	if((cmd = strrchr(argv[0], '/')) == NULL)
		cmd = argv[0];
	else
		cmd++;

	daemonize(cmd);

	if(already_running())
	{
		syslog(LOG_ERR, "daemon already running");
		exit(1);
	}

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if(sigaction(SIGHUP, &sa, NULL) < 0)
		err_quit("%s: cannot restore SIGHUP default");
	sigfillset(&mask);
	if((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)
		err_exit(err, "SIG_BLOCK error");

	err = pthread_create(&tid, NULL, thr_fn, 0);
	if(err != 0)
		err_exit(err, "cannot create thread");

	exit(0);
}
