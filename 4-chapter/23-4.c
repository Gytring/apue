 /*************************************************************************
	> File Name: 23-4.c
	> Author: free
	> Mail: fancanglang@foxmail.com 
	> Created Time: Tue 10 Jul 2018 06:05:36 PM CST
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(void)
{
	if(chdir("/tmp") < 0)
		err_sys("chdir failed");
	printf("chdir to /tmp succeed\n");
	exit(0);
}
