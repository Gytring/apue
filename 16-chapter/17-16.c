#include "apue.h"
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>

#define BUFLEN	128
#define QLEN	10

#ifndef	HOST_NAME_MAX	
#define HOST_NAME_MAX	256
#endif

//extern int initserver(int, const struct sockaddr *, socklen_t, int);
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

void serve(int sockfd)
{
	int		clfd;
	FILE	*fp;
	char	buf[BUFLEN];

	set_cloexec(sockfd);
	for(;;)
	{
		if((clfd = accept(sockfd, NULL, NULL)) < 0)
		{
			syslog(LOG_ERR, "rputimed: accept error: %s", strerror(errno));
			exit(1);
		}
		set_cloexec(clfd);
		if((fp = popen("/usr/bin/uptime", "r")) == NULL)
		{
			sprintf(buf, "errno: %s\n", strerror(errno));
			send(clfd, buf, strlen(buf), 0);
		}
		else
		{
			while(fgets(buf, BUFLEN, fp) != NULL)
				send(clfd, buf, strlen(buf), 0);
			pclose(fp);
		}
		close(clfd);
	}
}

int main(int argc, char *argv[])
{
	struct addrinfo		*ailist, *aip;
	struct addrinfo		hint;
	int					sockfd, err, n;
	char				*host;

	if(argc != 1)
		err_quit("usageL ruptimed");
	if((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
		n = HOST_NAME_MAX;
	if((host = (char *)malloc(n)) == NULL)
		err_sys("malloc errno");
	if(gethostname(host, n) < 0)
		err_sys("gethostname error");
	daemonize("rputimed");
	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = NULL;
	hint.ai_next = NULL;
	if((err = getaddrinfo(host, "rputimed", &hint, &ailist)) != 0)
	{
		syslog(LOG_ERR, "rputimed: getaddrinfo error: %s", gai_strerror(err));
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
