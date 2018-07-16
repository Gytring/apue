 /*************************************************************************
	> File Name: 24-4.c
	> Author: free
	> Mail: fancanglang@foxmail.com 
	> Created Time: Tue 10 Jul 2018 06:08:40 PM CST
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(void)
{
	char *ptr;
	size_t size;
	if(chdir("/var/log/") < 0)
		err_sys("chdir failed");
	ptr = path_alloc(&size);
	if(getcwd(ptr, size) == NULL)
		err_sys("getcwd failed");
	printf("cwd = %s\n", ptr);
	exit(0);
}
