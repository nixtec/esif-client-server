/* eduserver.h */
#ifndef _EDUSERVER_H_
#define _EDUSERVER_H_

#include <stdio.h>                  /* for standard input/output functions */
#include <stdlib.h>                 /* for exit and some useful functions */
#include <string.h>                 /* string manipulation */
#include <errno.h>                  /* error handling and logging */
#include <sys/socket.h>             /* for socket (client/server) interaction */
#include <sys/stat.h>
#include <sys/types.h>              /* type definitions */
#include <arpa/inet.h>              /* internet related functions */
#include <netinet/in.h>             /*   ....... */
#include <time.h>                   /* for time maintenance */
#include <signal.h>                 /* for signal handling */
#include <sys/wait.h>               /* for waiting */
#include <unistd.h>                 /* universal system specific functions */
#include <pthread.h>                /* thread specific functions */
#include <pwd.h>                    /* setuid(), etc */
#include <fcntl.h>

#include "password.h"               /* local password information */
#include "regdb.h"

#define LISTENQ 5                   /* maximum queue length for server */
#define MAXLINE 120                 /* maximum length of a line (message) */
#define SERV_PORT 7860              /* convert it to network byte order */

#define DIGEST "sha1"
#define MAGIC "UiPatyYZz49882454iIopzZ"   /* will be sha1 computed */

#define MAXDIRNAME 200
#define MAXFILENAME 20
#define LOGFILE "tmp/eduserver.log"

#define TXTROOT0 "var"
#define TXTROOT1 "var/regform"
#define TXTROOT2 "var/regform/text"
#define TXTROOT3 "var/regform/text/Rajshahi"

#define BDBROOT0 "var"
#define BDBROOT1 "var/regform"
#define BDBROOT2 "var/regform/bdb"
#define BDBROOT3 "var/regform/bdb/Rajshahi"

#define ERRROOT0 "var"
#define ERRROOT1 "var/regform"
#define ERRROOT2 "var/regform/error"
#define ERRROOT3 "var/regform/error/Rajshahi"

#define PASSDIR "var/regform/passwd"
#define PASS_DB "var/regform/passwd/pass.db"

#define TIMEOUT 60                  /* timeout in seconds */

/* not used right now */
#define USER "rajshahi"
#define GROUP "rajshahi"
/* ****************** */

#define DIRMODE 0750
#define FILEMODE 0640

#define MAXINFOSIZE 1024
#define MINTIMEOUT 5

#define SUCCESS 500

/* ERRORS ***/
#define E_ERROR 300
#define E_NO_COLLEGE 301
#define E_COLLEGE_DISABLED 302
#define E_INVALID_PASSWORD 303
#define E_TIME_NOSTART 304 /* time not yet started */
#define E_TIME_EXPIRED 305 /* time expired */

#define E_IMG_NOSUPPORT 310 /* image not supported */

/* function prototypes */
void manage_client(const char *remote_addr, int conn_id); /* handle client */
void handle_signal(int signo);      /* signal handler */

#endif  /* _EDUSERVER_H_ */
