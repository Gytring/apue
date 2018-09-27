#include "apue.h"
<<<<<<< HEAD
=======
#include <sys/socket.h>

int fd_pipe(int fd[2])
{
	return (socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
}
>>>>>>> 60cec8f759ac2875ba05bcd357f8188d4abe8c6c

