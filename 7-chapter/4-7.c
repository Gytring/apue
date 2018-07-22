#include "apue.h"

int main(int argc, char *argv[])
{
	int i;

	//for(i = 0; i < argc; i++)
	for(i = 0; argv[i] != NULL; i++)
	{
		printf("%d:\t%s\n", i, argv[i]);
	}

	return 0;
}
