 /*************************************************************************
	> File Name: 3-4.c
	> Author: free
	> Mail: fancanglang@foxmail.com 
	> Created Time: Tue 10 Jul 2018 10:55:29 AM CST
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(int argc, char *argv[])
{
	int i;
	struct stat buf;
	char *ptr;

	for(i = 1; i < argc; i++)
	{
		printf("%s: ", argv[1]);
		if(lstat(argv[i], &buf) < 0)
		{
			err_ret("lstat error");
			continue;
		}


		if(S_ISREG(buf.st_mode))
			ptr = "regular";
		else if(S_ISDIR(buf.st_mode))
			ptr = "diretory";
		else if(S_ISCHR(buf.st_mode))
			ptr = "character special";
		else if(S_ISBLK(buf.st_mode))
			ptr = "block special";
		else if(S_ISFIFO(buf.st_mode))
			ptr = "fifo";
		else if(S_ISLNK(buf.st_mode))
			ptr = "symbolic link";
		else if(S_ISSOCK(buf.st_mode))
			ptr = "socket";
		else
			ptr = "** unknow mode **";
		printf("%s\n", ptr);
	}

	exit(0);
}
