#include "listen.h"

#ifdef _WIN32
static SOCKET listenfd;
#else
static int listenfd;
#endif

void handle_signal(int signo);


/* reads a line from a socket */
/* NOT thread-safe */
#ifdef _WIN32
static ssize_t my_read(SOCKET fd, char *ptr)
#else
static ssize_t my_read(int fd, char *ptr)
#endif
{
  static int read_cnt = 0;
  static char *read_ptr;
  static char read_buf[MAXLINE];


//  PRINTF("***debug: IN my_read: ***\n");

  if (read_cnt <= 0) {
again:
#ifdef _WIN32
    if ((read_cnt = recv(fd, read_buf, sizeof(read_buf), 0)) < 0) {
#else
    if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
#endif
      if (errno == EINTR)
	goto again;
      return (-1);
    } else if (read_cnt == 0)
      return (0);
    read_ptr = read_buf;
  }
  read_cnt--;
  *ptr = *read_ptr++;

//  PRINTF("***debug: OUT my_read: ***\n");

  return (1);
}

#ifdef _WIN32
ssize_t readline(SOCKET fd, void *vptr, size_t maxlen)
#else
ssize_t readline(int fd, void *vptr, size_t maxlen)
#endif
{
  int n, rc;
  char c, *ptr;

//  PRINTF("***debug: IN readline: ***\n");

  ptr = vptr;
  for (n = 1; n < maxlen; n++) {
    if ((rc = my_read(fd, &c)) == 1) {
      *ptr++ = c;
      if (c == '\n')
	break; /* newline is stored, like fgets() */
    } else if (rc == 0) {
      if (n == 1)
	return (0); /* EOF, no data */
      else
	break; /* EOF, some data was read */
    } else
	return (-1); /* error, errno set by read() */
  }
  *ptr = 0; /* null terminate like fgets() */

//  PRINTF("***debug: OUT readline: ***\n");

  return (n);
}

#ifdef _WIN32
ssize_t readn(SOCKET fd, void *vptr, size_t n)
#else
ssize_t readn(int fd, void *vptr, size_t n)
#endif
{
  size_t nleft;
  ssize_t nread;
  char *ptr;

  ptr = vptr;
  nleft = n;
  while (nleft > 0) {
#ifdef _WIN32
    if ((nread = recv(fd, ptr, nleft, 0)) < 0) {
#else
    if ((nread = read(fd, ptr, nleft)) < 0) {
#endif
      if (errno == EINTR)
	nread = 0; /* call read() again */
      else
	return (-1);
    } else if (nread == 0)
      break; /* EOF */
    nleft -= nread;
    ptr += nread;
  }
  return (n - nleft);
}

#ifdef _WIN32
ssize_t writen(SOCKET fd, const void *vptr, size_t n)
#else
ssize_t writen(int fd, const void *vptr, size_t n)
#endif
{
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;

  ptr = vptr;
  nleft = n;
  while (nleft > 0) {
#ifdef _WIN32
    if ((nwritten = send(fd, ptr, nleft, 0)) <= 0) {
#else
    if ((nwritten = write(fd, ptr, nleft)) <= 0) {
#endif
      if (errno == EINTR)
	nwritten = 0; /* call write() again */
      else
	return (-1); /* error */
    }
    nleft -= nwritten;
    ptr += nwritten;
  }
  return (n);
}


int main(int argc, char **argv)
{
  int connfd;
  struct sockaddr_in servaddr;
  int i;
  socklen_t clilen;
  int ret;

  (void) argc;
  (void) argv;

  char rbuf[MAXLINE];
  /* options supported:
   * --acceptimage             => clients may send images
   */
  if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "main: socket: %s\n", strerror(errno));
//    fflush(stdout);
    exit(EXIT_FAILURE);
  }


  memset(&servaddr, 0, sizeof(struct sockaddr_in)); /* zero it */
  servaddr.sin_family = PF_INET; /* */
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); /* accept from anywhere */
  servaddr.sin_port = htons(SERV_PORT); /* port to listen on */

  if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) { /* error */
    fprintf(stderr, "main: bind: %s\n", strerror(errno));
//    fflush(stdout);
    close(listenfd);
    exit(EXIT_FAILURE);
  }

  /* now listen for connections */
  if (listen(listenfd, LISTENQ) == -1) {
    fprintf(stderr, "main: listen: %s\n", strerror(errno));
//    fflush(stdout);
    close(listenfd);
    exit(EXIT_FAILURE);
  }

//  setsid(); /* be daemon */
  for (i=1; i<=30; i++) {
    switch(i) {
      case SIGHUP:
      case SIGINT:
      case SIGTERM:
	signal(i, handle_signal); /* exit program */
	break;
      default:
	signal(i, SIG_IGN);
	break;
    }
  }


  fprintf(stderr, "main: Waiting for connections ...\n");
//  fflush(stdout);

  while(1) {
    clilen = 0;
    if ((connfd = accept(listenfd, (struct sockaddr *) NULL, &clilen)) == -1) {
      if (errno == EINTR) { /* signal handled before connection */
	fprintf(stderr, "main: accept: EINTR: %s, ignoring\n", strerror(errno));
//	fflush(stdout);
	continue;
      }
      else {
	fprintf(stderr, "main: accept: %s, continuing\n", strerror(errno));
//	fflush(stdout);
	continue;
      }
    }

    readline(connfd, rbuf, sizeof(rbuf));
    printf("Message from server : %s\n", rbuf);

    close(connfd); /* close socket */
  } /* while() */

  return 0;
}

void handle_signal(int signo)
{
  printf("Signal handled : %d, exiting...\n", signo);

  close(listenfd);
  exit(signo);
}
