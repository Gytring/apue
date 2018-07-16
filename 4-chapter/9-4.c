 /*************************************************************************
	> File Name: 9-4.c
	> Author: free
	> Mail: fancanglang@foxmail.com 
	> Created Time: Tue 10 Jul 2018 11:30:35 AM CST
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <fcntl.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int main(void)
{
	umask(0);
	if(creat("foo", RWRWRW) < 0)
		err_sys("creat error fo foo");
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if(creat("bar", RWRWRW) < 0)
		err_sys("creat error for bar");
	exit(0);
}
