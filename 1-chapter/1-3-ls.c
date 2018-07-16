 /*************************************************************************
	> File Name: 1-3-ls.c
	> Author: free
	> Mail: fancanglang@foxmail.com 
	> Created Time: Sun 08 Jul 2018 02:00:04 PM CST
 ************************************************************************/
#include <dirent.h>
#include <stdio.h>
#include "apue.h"

int main(int argc, char *argv[])
{
	DIR *dp;
	struct dirent *dirp;

	if(argc < 2)
	{
		err_quit("usage:ls diretory_name");
	}

	if((dp = opendir(argv[1])) == NULL)
		err_sys("can't open %s\n", argv[1]);

	while((dirp = readdir(dp)) != NULL)
		printf("%s\t", dirp->d_name);

	putchar('\n');
	closedir(dp);
	exit(0);
}
