#include "apue.h"
#include <sys/wait.h>
#include "5-8.h"

int main(int status)
{
	if((status = system("date")) < 0)
		err_sys("system() error");

	pre_exit(status);

	if((status = system("nosuchcommand")) < 0)
		err_sys("ststem() error");

	pre_exit(status);

	if((status = system("who; exit 44")) < 0)
		err_sys("ststem() error");

	pre_exit(status);

	exit(0);
}
