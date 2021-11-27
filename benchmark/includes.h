/* common headers for all */
/* all of the sources only include this */
#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include <stdio.h>
#include <string.h>
#include <strings.h>          /* strncasecmp(), etc */
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

/* mingw <unistd.h> is buggy */
#ifndef _WIN32
#include <unistd.h>
#endif

//#ifdef _WIN32
#include <openssl/sha.h>    /* currently i'm using only sha1() routines */
//#else
//#include <openssl/evp.h>   /* encryption routines */
//#endif

/* all includes should be here */
#ifdef _WIN32

#include <windows.h>
#include <wincon.h>
/* network routines */
#include <winsock.h>       /* it includes everything necessary for socket programs */

#else

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#endif

#include <gtk/gtk.h>

/* all #define s should be here */
#include "errors.h"
#include "defines.h"

#endif     /* _INCLUDES_H_ */
