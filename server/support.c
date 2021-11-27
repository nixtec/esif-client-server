#include "support.h"

ssize_t readn(int fd, void *vptr, size_t n)
{
  size_t nleft;
  ssize_t nread;
  char *ptr;

  ptr = vptr;
  nleft = n;
  while (nleft > 0) {
    if ((nread = read(fd, ptr, nleft)) < 0) {
      if (errno == EINTR)
	nread = 0;
      else
	return (-1);
    } else if (nread == 0)
      break; /* EOF */
    nleft -= nread;
    ptr += nread;
  }
  return (n - nleft);
}

ssize_t writen(int fd, const void *vptr, size_t n)
{
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;

  ptr = vptr;
  nleft = n;
  while (nleft > 0) {
    if ((nwritten = write(fd, ptr, nleft)) <= 0) {
      if (errno == EINTR)
	nwritten = 0; /* and call write() again */
      else
	return (-1); /* error */
    }
    nleft -= nwritten;
    ptr += nwritten;
  }
  return (n);
}

/* readline: reads a line one character at a time
 * this is inefficient
 */
#ifdef ___NO___
ssize_t readline(int fd, void *vptr, size_t maxlen)
{
  ssize_t n, rc;
  char c, *ptr;

  ptr = vptr;
  for (n = 1; n < (ssize_t) maxlen; n++) {
again:
    if ((rc = read(fd, &c, 1)) == 1) {
      *ptr++ = c;
      if (c == '\n')
	break; /* newline is stored, like fgets() */
    } else if (rc == 0) {
      if (n == 1)
	return (0); /* EOF, no data read */
      else
	break; /* EOF, some data read */
    } else {
      if (errno == EINTR)
	goto again;
      return (-1);
    }
  }
  *ptr = 0; /* nul terminates like fgets() */
  return (n);
}
#endif

static pthread_key_t rl_key;
static pthread_once_t rl_once = PTHREAD_ONCE_INIT;

static void readline_destructor(void *ptr)
{
  free(ptr);
}

static void readline_once(void)
{
  int ret;
  if ((ret = pthread_key_create(&rl_key, readline_destructor)) != 0) {
    printf("readline_once: pthread_key_create: %s\n", strerror(ret));
  }
}

typedef struct {
  int rl_cnt;  /* initialize to 0 */
  char *rl_bufptr; /* initialize to rl_buf */
  char rl_buf[MAXLINE];
} Rline;


static ssize_t my_read(Rline *tsd, int fd, char *ptr)
{
  if (tsd->rl_cnt <= 0) {
again:
    if ((tsd->rl_cnt = read(fd, tsd->rl_buf, MAXLINE)) < 0) {
      if (errno == EINTR)
	goto again;
      return (-1);
    } else if (tsd->rl_cnt == 0)
      return (0);
    tsd->rl_bufptr = tsd->rl_buf;
  }
  tsd->rl_cnt--;
  *ptr = *tsd->rl_bufptr++;
  return (1);
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
  int n, rc;
  char c, *ptr;
  Rline *tsd;

  int ret;

  ret = pthread_once(&rl_once, readline_once);
  if (ret != 0) {
    printf("readline: pthread_once: %s\n", strerror(ret));
  }

  if ((tsd = pthread_getspecific(rl_key)) == NULL) {
    tsd = calloc(1, sizeof(Rline)); /* init to 0 */
    if ((ret = pthread_setspecific(rl_key, tsd)) != 0) {
      printf("readline: pthread_setspecific: %s\n", strerror(ret));
    }
  }

  ptr = vptr;
  for (n = 1; n < (int) maxlen; n++) {
    if ((rc = my_read(tsd, fd, &c)) == 1) {
      *ptr++ = c;
      if (c == '\n') /* store the newline, like fgets() */
	break;
    } else if (rc == 0) {
      if (n == 1)
	return (0); /* EOF, no data read */
      else
	break; /* EOF, some data read */
    } else
      return (-1);
  }
  *ptr = 0; /* nul terminate, like fgets() */
  return (n);
}
