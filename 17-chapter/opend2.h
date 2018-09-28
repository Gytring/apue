#include "apue.h"
#include <error.h>

#define	CS_OPEN	"/tmp/opend.socket"		/* well-known name */
#define	CL_OPEN	"open"					/* client's request for server */

extern	int		debug;
extern	char	errmsg[];
extern	int		oflag;
extern	char	*pathname;

typedef	struct	{
	int		fd;
	uid_t	uid;
} Client;

extern	Client	*client;
extern	int		client_size;	

int		cli_args(int, char **);
int		client_add(int, uid_t);
void	client_del(int);
void	loop(void);
void	handle_request(char *, int, int, uid_t);
