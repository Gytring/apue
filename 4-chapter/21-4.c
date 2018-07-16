 /*************************************************************************
	> File Name: 21-4.c
	> Author: free
	> Mail: fancanglang@foxmail.com 
	> Created Time: Tue 10 Jul 2018 01:09:50 PM CST
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <fcntl.h>

int main(int argc, int *argv[])
{
	int i,fd;
	struct stat statbuf;
	struct timespec times[2];

	for(i = 1; i < argc; i++)
	{
		if(stat(argv[i], &statbuf) < 0)
		{
			err_ret("%s: stat error", argv[i]);
			continue;
		}
		if((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0)
		{
			err_ret("%s: open error", argv[i]);
			continue;
		}
		times[0] = statbuf.st_atim;
		times[1] = statbuf.st_mtim;
		if(futimens(fd, times) < 0)
			err_ret("%s: futimens error", argv[i]);
		close(fd);
	}
	exit(0);
}
