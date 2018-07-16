 /*************************************************************************
	> File Name: 1-6-getpid.c
	> Author: free
	> Mail: fancanglang@foxmail.com 
	> Created Time: Sun 08 Jul 2018 02:46:42 PM CST
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(void)
{
	printf("hello world from process ID %ld\n", (long)getpid());
	exit(0);
}
