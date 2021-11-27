#include "includes.h"
#include "defines.h"

#define MAXN 16384     /* max #bytes to request from server */

#define HOST "127.0.0.1"
#define PORT SERV_PORT

int main(int argc, char **argv)
{
  int i, j, fd, nchildren, nloops, nbytes;
  pid_t pid;
  ssize_t n;

  for (i = 0; i < nchildren; i++) {
    if ((pid = fork()) == 0) { /* child */
      for (j = 0; j < nloops; j++) {
	fd = init_connection(HOST, PORT);
	if (fd <= 0)
	  exit(0);
      }
    }
    /* parent loops around fork() again */
  }

  while (wait(NULL) > 0) ; /* wait for all children */
  if (errno != ECHILD)
    printf("error wait: %s\n", strerror(errno));

  return 0;
}
