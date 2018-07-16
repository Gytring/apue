 /*************************************************************************
	> File Name: 8-4.c
	> Author: free
	> Mail: fancanglang@foxmail.com 
	> Created Time: Tue 10 Jul 2018 11:21:09 AM CST
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include "apue.h"

int main(int argc, char *argv[])
{
	if(argc != 2)
		err_quit("usage: ./8-4 <pathname>");
	if(access(argv[1], R_OK) < 0)
		err_ret("access err for %s", argv[1]);
	else
		printf("read access OK\n");
	if(open(argv[1], O_RDONLY) < 0)
		err_ret("open error for %s", argv[1]);
	else
		printf("open for reading OK\n");

	exit(0);
}
