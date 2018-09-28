#include "apue.h"
#include "termios.h"
#include <errno.h>

static struct termios	save_termios;
static int				ttysavefd = -1;
static enum { RESET, RAM, CBREAK} ttystat = RESET;

int tty_cbreak(int fd)		/* put terminal into a cbreak mode */
{

}
