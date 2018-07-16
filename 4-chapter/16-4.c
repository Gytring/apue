 /*************************************************************************
	> File Name: 16-4.c
	> Author: free
	> Mail: fancanglang@foxmail.com 
	> Created Time: Tue 10 Jul 2018 12:59:47 PM CST
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <fcntl.h>

int main(void)
{
	if(open("tempfile", O_RDWR) < 0)
		err_sys("open error");
	if(unlink("tempfile") < 0)
		err_sys("unlink error");
	printf("file unlink\n");

	sleep(15);
	printf("done\n");
	exit(0);
}
