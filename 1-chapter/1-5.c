 /*************************************************************************
	> File Name: 1-5.c
	> Author: free
	> Mail: fancanglang@foxmail.com 
	> Created Time: Sun 08 Jul 2018 02:33:37 PM CST
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(void)
{
	int c;

	while((c = getc(stdin)) != EOF)
		if(putc(c, stdout) == EOF)
			err_sys("output error");

	if(ferror(stdin))
		err_sys("input error");

	exit(0);
}
