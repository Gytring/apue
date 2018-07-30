#include "apue.h"
#include <sys/stat.h>

#include "5-8.h"

int main(int argc, char *argv[])
{
	int status;

	if(argc < 2)
		err_sys("command-line argument required");

	if((status = system(argv[1])) < 0)
		err_sys("system() error");

	pre_exit(status);
	exit(0);
}
