 /*************************************************************************
	> File Name: 1-4-cpfile.c
	> Author: free
	> Mail: fancanglang@foxmail.com 
	> Created Time: Sun 08 Jul 2018 02:18:36 PM CST
 ************************************************************************/
#include "apue.h"
#include <stdio.h>
#include <unistd.h>
#define BUFFIZE 4096

int main(void)
{
	int n;
	char buf[BUFFIZE];

	while((n = read(STDIN_FILENO, buf, BUFFIZE)) > 0)  
		if(write(STDOUT_FILENO, buf, n) != n)
			err_sys("write error");

	if(n < 0)
		err_sys("read error");

	exit(0);
}
