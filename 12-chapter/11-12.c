#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define		MAXSTRINGSZ 4096

static	char envbuf[MAXSTRINGSZ];

extern	char	**environ;

char *getenv(const char *name)
{
	int i, len;

	len = strlen(name);
	for(i = 0; environ[i] != NULL; i++)
	{
		if((strncmp(name, environ[i], len)== 0) && (environ[i][len] == '='))
		{
			strncpy(envbuf, &environ[i][len+1], MAXSTRINGSZ-1);
			return(envbuf);
		}
	}
	return(NULL);
}

int main(void)
{
	char pathname[] = ".";
	printf("%s\n", getenv(pathname));
	exit(0);
}
