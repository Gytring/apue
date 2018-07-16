 /*************************************************************************
	> File Name: 12-4.c
	> Author: free
	> Mail: fancanglang@foxmail.com 
	> Created Time: Tue 10 Jul 2018 11:43:07 AM CST
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(void)
{
	struct stat statbuf;

	if(stat("foo", &statbuf) < 0)
		err_sys("stat error for foo");
	if(chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
		err_sys("chmod error for foo");

	if(chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
		err_sys("chmod error for bar");

	exit(0);
}
