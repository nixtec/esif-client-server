/* listen.h */
#ifndef _EDUSERVER_H_
#define _EDUSERVER_H_

#include <stdio.h>                  /* for standard input/output functions */
#include <stdlib.h>                 /* for exit and some useful functions */
#include <signal.h>                 /* for signal handling */
#include <errno.h>

#ifndef _WIN32
#include <sys/socket.h>             /* for socket (client/server) interaction */
#include <sys/stat.h>
#include <sys/types.h>              /* type definitions */
#include <arpa/inet.h>              /* internet related functions */
#include <netinet/in.h>             /*   ....... */
#include <unistd.h>                 /* universal system specific functions */
#else
#include <windows.h>
#include <winsock.h>
#endif



#define LISTENQ 5                   /* maximum queue length for server */
#define MAXLINE 120                 /* maximum length of a line (message) */
#define SERV_PORT 9999              /* convert it to network byte order */

#endif  /* _EDUSERVER_H_ */
