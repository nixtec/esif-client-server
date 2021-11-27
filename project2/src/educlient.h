/* educlient.h */
#include <stdio.h>                  /* for standard input/output functions */
#include <stdlib.h>                 /* for exit and some useful functions */
#include <string.h>                 /* string manipulation */
#include <errno.h>                  /* error handling and logging */
#include <sys/socket.h>             /* for socket (client/server) interaction */
#include <sys/types.h>              /* type definitions */
#include <arpa/inet.h>              /* internet related functions */
#include <netinet/in.h>             /*   ....... */
#include <time.h>                   /* for time maintenance */
#include <signal.h>                 /* for signal handling */
#include <sys/wait.h>               /* for waiting */
#include <unistd.h>                 /* universal system specific functions */
#include <fcntl.h>

#define DIGEST "sha1"               /* message digest name */
#define MAGIC "UiPatyYZz49882454iIopzZ"   /* will be sha1 computed */

#define MAXLINE 80                   /* maximum length of a line (message) */
#define SERV_PORT 7860                /* convert it to network byte order */

#define LOGFILE "/tmp/educlient.log"

void handle_signal(int signo);

