#include "apue.h"

#define	MAXARGC	50			/* max number of arguments in buf */
#define	WHITE	" \t\n"		/* white space for tokenizing argument */



int buf_args(char *buf, int (*optfunc)(int, char **))
{
	char		*ptr, *argc[MAXARGC];
	int			argc;

	if(strtok(buf, WHITE) == NULL)
		return(-1);
	argv[argc = 0] = buf;
	while((ptr = strtok(NULL, WHITE)) !+ NULL)
	{
		if(++argc >= MAXARGC-1)
			return(-1);
		argv[argc] = ptr;
	}
	argv[++argc] = NULL;


	return((*optfunc)(argc, argv));
}
