#include "includes.h"
#include "defines.h"
#include "myfuncs.h"

#define MAXN 16384     /* max #bytes to request from server */

#define HOST "127.0.0.1"
#define PORT SERV_PORT

//static char *login = "0110";
//static char *pass = "ttippp";

unsigned char magic[SHA_DIGEST_LENGTH];
void init_magic(const char *str, unsigned char *md_value);

int main(int argc, char **argv)
{
  int i, j, nchildren, nloops;
  pid_t pid;
  char session[5];
  char college[5];
  char dir[MAXDIRNAME];
//  int port;

  nchildren = 1; /* how many colleges */
  nloops = 1; /* how many sessions each college will send info for */

  memset(magic, 0, sizeof(magic));
  init_magic(MAGIC, magic);

//  while(1) { /* do all the night */
  for (i = 0; i < nchildren; i++) {
    sprintf(college, "%04d", 1025+i);
//    printf("college : %s\n", college);
    pid = fork();
    switch(pid) {
      case 0:
	for (j = 0; j < nloops; j++) {
	  sprintf(session, "%04d", 2005+j);
	  sprintf(dir, "/home/ayub/Rajshahi/%s/INSTITUTE_%s", session, college);
	  printf("%s\n", dir);
	  apply_forms(HOST, dir, college, session);
	}
	exit(0);
	break;
      default:
	printf("parent: waiting...\n");
	while (wait(NULL) > 0) ; /* free up process table */
	if (errno != ECHILD)
	  printf("error wait: %s\n", strerror(errno));
	break;
    }
  } /* for(i ...) */
//  } /* while(1) */

  return 0;
}

void init_magic(const char *str, unsigned char *md_value)
{
  SHA_CTX shactx;

  SHA1_Init(&shactx);
  SHA1_Update(&shactx, str, strlen(str));
  SHA1_Final(md_value, &shactx);
}
