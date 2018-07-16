 /*************************************************************************
	> File Name: 1-9-uid.c
	> Author: free
	> Mail: fancanglang@foxmail.com 
	> Created Time: Sun 08 Jul 2018 03:21:37 PM CST
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(void)
{
	printf("uid=%d\tgid=%d\n", getuid(), getgid());
	exit(0);
}
