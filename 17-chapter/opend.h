#include	"apue.h"
#include	<errno.h>

#define		CL_OPEN	"open"	/* client's request for server */

extern	char	errmag[];	/* open() flag string to return to client to client */
extern	int		oflag;
extern	char	*pathname;	/* of file to open() for client	*/

int		cli_args(int, char **);
void	handle_request(char	*, int, int);
