#include "apue.h"
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

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

int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
	int fd;
	int err = 0;

	if((fd = socket(addr->sa_family, type, 0)) < 0)
		return(-1);
	if(bind(fd, addr, alen) < 0)
		goto errout;
	if(type == SOCK_STREAM || type == SOCK_SEQPACKET)
	{
		if(listen(fd, qlen) < 0)
			goto errout;
	}
	return(fd);

errout:
	err = errno;
	close(fd);
	errno = err;
	return(-1);
}

int set_cloexec(int fd)
{
	int val;

	if((val = fcntl(fd, F_GETFD, 0)) < 0)
		return(-1);

	val |= FD_CLOEXEC;
	return(fcntl(fd, F_SETFD, val));
}

void serve(int sockfd)
{
	int		clfd, status;
	pid_t	pid;

	set_cloexec(sockfd);
	for(;;)
	{
		if((clfd = accept(sockfd, NULL, NULL)) < 0)
		{
			syslog(LOG_ERR, "cuptimed: accept error: %s", strerror(errno));
			exit(1);
		}
		if((pid = fork()) < 0)
		{
			syslog(LOG_ERR, "ruptimed: fork error: %s", strerror(errno));
			exit(1);
		}
		else if(pid == 0)
		{
			if(dup2(clfd, STDOUT_FILENO) != STDOUT_FILENO || dup2(clfd, STDERR_FILENO) != STDERR_FILENO)
			{
				syslog(LOG_ERR, "ruptimed: unexpected error");
				exit(1);
			}
			close(clfd);
			execl("/usr/bin/uptime", "uptime", (char *)0);
			syslog(LOG_ERR, "ruptimed: unexpected return from exec: %s", strerror(errno));
		}
		else
		{
			close(clfd);
			waitpid(pid, &status, 0);
		}
	}
}

int main(int argc, char *argv[])
{
	struct addrinfo		*ailist, *aip;
	struct addrinfo		hint;
	int					sockfd, err, n;
	char				*host;

	if(argc != 1)
		err_quit("usage: ruptimed");
	if((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
		n = HOST_NAME_MAX;
	if((host = (char *)malloc(n)) == NULL)
		err_sys("malloc error");
	if(gethostname(host, n) < 0)
		err_sys("gethostname error");
	daemonize("ruptimed");
	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if((err = getaddrinfo(host, "ruptimed", &hint, &ailist)) != 0)
	{
		syslog(LOG_ERR, "ruptimed getaddrinfo error: %s", gai_strerror(err));
		exit(1);
	}
	for(aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		if((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0)
		{
			serve(sockfd);
			exit(0);
		}
			
	}
	exit(1);
}
