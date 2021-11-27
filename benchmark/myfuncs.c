#include "includes.h"
#include "myfuncs.h"
/* socket timeout */
#ifndef TIMEOUT
#define TIMEOUT 60
#endif


#define E_NO_COLLEGE 301
#define E_COLLEGE_DISABLED 302
#define E_INVALID_PASSWORD 303
#define E_TIME_NOSTART 304 /* time not yet started */
#define E_TIME_EXPIRED 305 /* time expired (not allowed to fill form) */



//#ifdef _WIN32
//typedef int ssize_t;
//#endif

static ssize_t my_read(int fd, char *ptr);

ssize_t readline(int fd, void *vptr, size_t maxlen);

ssize_t readn(int fd, void *vptr, size_t n);

ssize_t writen(int fd, const void *vptr, size_t n);

static int sockfd; /* connection handle */

int init_connection(const char *host, int port)
{
  int sock;
  struct sockaddr_in servaddr; /* server socket address structure */
//  int i;

//  int errval = 0;

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = PF_INET;        /* socket address family to use */
  servaddr.sin_port = htons(port); /* port of the server */

  if (inet_aton(host, &servaddr.sin_addr) == 0) {
    printf("invalid address supplied\n");
    return -1;
  } /* end if */

  if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    printf("socket: %s\n", strerror (errno));
    return -1;
  }

//  printf("Connecting to server : %-30s", host);
  if (connect(sock, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1) {
    close(sock);
    printf("[FAILED]\n");
    printf("connect: %s\n", strerror(errno));
    return -1;
  }
  printf("[  OK  ]\n");

  return sock;
}

char *apply_forms(const char *host, const char *dir, const char *college, const char *session)
{
  FILE *fp;
  DIR *info = NULL;
  struct dirent *file;
  char full_filename[MAXDIRNAME+MAXFILENAME];
  int len;
  char sec[2];
  char gen[7];
  int roll;
  int sent;
  char buffer[MAXLINE]; /* only first line will be read */

//  GtkWidget *ptr;
  int success = 0;
  int failure = 0;

//  int val;
  int ret = 0;
  int nwrite;

  char pass[20];

  sprintf(pass, "%s%s", session, college);

  sockfd = init_connection(host, SERV_PORT);
  if (sockfd == -1) {
    return NULL;
  }
  else if (authenticate(college, pass)) {
//    printf("Authentication failure\n");
//    show_message("Authentication Failure");
    close(sockfd);
    return NULL;
  }

  info = opendir(dir);
  if (!info) {
    printf("opendir : %s : %s\n", dir, strerror(errno));
    goto write_quit;
  }
  /* directory opened */
  while ((file = readdir(info))) {
    if (file->d_name[0] == '.')
      continue;
    len = strlen(file->d_name);
    if (!(len == 15 || len == 17))
      continue; /* skip invalid files */
    if (strcmp(&file->d_name[len-4], ".txt"))
      continue; /* skip non-text files */
    strcpy(full_filename, dir);
    if (dir[strlen(dir)-1] != '/') /* add '/' if not present */
      strcat(full_filename, "/");
    strcat(full_filename, file->d_name);

    printf("Opening %s\n", full_filename);
    fp = fopen(full_filename, "r"); /* just for checking if it's sent */
    if (!fp) {
      printf("fopen : %s : %s\n", full_filename, strerror(errno));
      continue;
    }
    fgets(buffer, 30, fp);

    sscanf(buffer, "%[^:]:%[^:]:%d:%d", sec, gen, &roll, &sent);
    fclose(fp);
    if (!sent) { /* not sent */
      sprintf(buffer, "FILENAME %s\n", file->d_name);
      printf(buffer, "FILENAME |%s|\n", file->d_name);
      nwrite = strlen(buffer);
      if ((ret = writen(sockfd, buffer, nwrite)) != nwrite) {
	sprintf(buffer, "ERROR : FILENAME : send : %s", strerror(errno));
	show_message(strerror(errno));
	close(sockfd);
	show_message(buffer);
	ret = 1;
	goto free_mem;
      }
      if (apply_form(full_filename) == 0)
	success++;
      else
	failure++;
    }
  }

write_quit:
  if (sockfd > 0)
    writen(sockfd, "QUIT\n", 5);

free_mem:
  closedir(info);
  close(sockfd);
  return NULL;
}

/* apply individual form */
int apply_form(const char *filename)
{
  FILE *fp = NULL;
//  char base[20]; /* name without extension */
  char buffer[MAXLINE]; /* read buffer */
  char sec[2]; /* section */
  char gen[7]; /* gender */
  int roll;
  int sent;
  char st_name[41]; /* student's name */
  char rbuf[20]; /* socket read buffer */

  int ret;
  int err = 0;
  int nwrite;

//  printf("Apply form : %s\n", filename);
  
//  freopen (LOGFILE, "w", stdout);
  

  memset(rbuf, 0, sizeof(rbuf));
  memset(buffer, 0, sizeof(buffer));

  fp = fopen(filename, "r"); /* open for reading */
  if (!fp) {
    printf("%s : ", filename);
    printf("fopen: %s: %s\n", filename, strerror(errno));
    show_message(strerror(errno));
    err = 2;
    return 1;
  }

  while (fgets(buffer, sizeof(buffer), fp)) {
    nwrite = strlen(buffer);
    if (writen(sockfd, buffer, nwrite) != nwrite) {
      printf("error: send: %s\n", strerror(errno));
      fclose(fp);
      return 3;
    }
  }
  if (writen(sockfd, ".\n", 2) < 2) {
    printf("ERROR : send : %s\n", strerror(errno));
    fclose(fp);
    return 1;
  }

  /* readline() sets extra terminating null byte to buffer */
  printf("Waiting for feedback from server\n");
  if ((ret = readline(sockfd, rbuf, 20)) != 4) {
    printf("[FAILED]\n");
//    perror("readline");
    return 1;
  }
  if (!strncmp(rbuf, "500\n", 4)) {
    printf("SERVER %s", rbuf);
    err = 0;
  }
  else {
    printf("SERVER %s", rbuf);
    err = 1;
  }

//  sleep(1);
//  err = 0; /* for now i'm assuming it */

//finalize:
  if (fp)
    fclose(fp);

  if (err == 0) {
    /* sent successfully, feedback got */
    /* update the sent flag to the file */
    fp = fopen(filename, "r+");
    if (!fp)
      return 1;
    fgets(buffer, sizeof(buffer), fp); /* read first line (including newline ) */
    sscanf(buffer, "%[^:]:%[^:]:%d:%d:%[^\n]", sec, gen, &roll, &sent, st_name);
    rewind(fp); /* again reset */
    fprintf(fp, "%s:%s:%04d:%d:%s\n", sec, gen, roll, 1, st_name); /* sent */

    /* everything is done successfully */
    fclose(fp);
    printf("SUCCESS\n");

    return 0;
  }
  else
    return 1;
}

void show_message(const char *str)
{
  printf("%s\n", str);
}

int authenticate(const char *username, const char *pass)
{
  char buffer[80];
//  EVP_MD_CTX mdctx; /* message digest context */
//  const EVP_MD *md;
//  static unsigned char md_value[EVP_MAX_MD_SIZE+1];
//  int md_len;
  SHA_CTX shactx;
  static unsigned char md_value[SHA_DIGEST_LENGTH];

  int nwrite;
  int ret;
  int errval;

//  GtkWidget *ptr;
//  GString *msg = NULL;

//  printf("EVP_MAX_MD_SIZE = %d\n", EVP_MAX_MD_SIZE);

  if (!username || strlen(username) != 4) { /* college code */
    show_message("College code must be 4 digits");
    return 1;
  }
  /*
  if (!pass || strlen(pass) < 6) {
    show_message("Password must be at least 6 characters");
    return 2;
  }
  */



//  sleep(3);
  nwrite = SHA_DIGEST_LENGTH;
//  printf("Sending magic ... ");
  if ((ret = writen(sockfd, magic, nwrite)) != nwrite) {
    printf("[FAILED]\n");
    return 1;
  }
  printf("[OK]\n");
  printf("Waiting for feedback ... ");
  if ((ret = readline(sockfd, buffer, MAXLINE)) != 4) {
    printf("[FAILED]\n");
    return 1;
  }
  printf("[  OK  ]\n");

  if (!strncmp(buffer, "500\n", 4)) {
    printf("Magic OK\n");
  }
  else {
    printf("Magic failure\n");
    return 1;
  }

  /* first send username */
  sprintf(buffer, "USER %s\n", username);
  nwrite = strlen(buffer);
  printf("Sending college code ... ");

  if ((ret = writen(sockfd, buffer, nwrite)) != nwrite) {
    printf("[FAILED]\n");
    return 1;
  }
  printf("[OK]\n");

  printf("Waiting for reply ...");
  if ((ret = readline(sockfd, buffer, MAXLINE)) < 4) {
    printf("[FAILED]\n");
    return 1;
  }
  printf("[  OK  ]\n");
  errval = strtol(buffer, (char **) NULL, 10);
  switch(errval) {
    case E_NO_COLLEGE:
      printf("E_NO_COLLEGE\n");
      show_message("No such college code in server");
      return 1;
      break;
    case E_COLLEGE_DISABLED:
      printf("E_COLLEGE_DISABLED\n");
      show_message("College Disabled");
      return 1;
      break;
    case E_TIME_NOSTART:
      printf("E_TIME_NOSTART\n");
      return 1;
      break;
    case E_TIME_EXPIRED:
      printf("E_TIME_EXPIRED\n");
      return 1;
      break;
    case 500:
      printf("[  OK  ]\n");
      break;
    default:
      printf("Unknown error\n");
      printf("SERVER: %d\n", errval);
      return 1;
      break;
  }

  memset(md_value, 0, sizeof(md_value));
  strcpy(buffer, "PASS ");

  SHA1_Init(&shactx);
  SHA1_Update(&shactx, pass, strlen(pass));
  SHA1_Final(md_value, &shactx);
  memcpy(buffer+5, md_value, SHA_DIGEST_LENGTH); /* copy password to buffer */

  nwrite = 5+SHA_DIGEST_LENGTH;
//  printf("buff = %s\n", buffer);
//  printf("len = %d\n", nwrite);
  printf("Writing password to socket ... ");
  if ((ret = writen(sockfd, buffer, nwrite)) != nwrite) {
    printf("[FAILED]\n");
    return 1;
  }
  printf("[  OK  ]\n");
  printf("Waiting for reply ...");
  if ((ret = readline(sockfd, buffer, MAXLINE)) != 4) {
    printf("[FAILED]\n");
    return 1;
  }
  printf("[  OK  ]\n");
  errval = strtol(buffer, (char **) NULL, 10);
  switch(errval) {
    case E_INVALID_PASSWORD:
      printf("Invalid password\n");
      show_message("Invalid Password");
      return 1;
      break;
    default:
      printf("SERVER: %d\n", errval);
      break;
  }

  return 0;
}

/* NOT thread-safe */
static ssize_t my_read(int fd, char *ptr)
{
  static int read_cnt = 0;
  static char *read_ptr;
  static char read_buf[MAXLINE];

  if (read_cnt <= 0) {
again:
    if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
      if (errno == EINTR)
	goto again;
      return (-1);
    } else if (read_cnt == 0)
      return (0);
    read_ptr = read_buf;
  }
  read_cnt--;
  *ptr = *read_ptr++;
  return (1);
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
  int n, rc;
  char c, *ptr;

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
  return (n);
}

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
	nwritten = 0; /* call write() again */
      else
	return (-1); /* error */
    }
    nleft -= nwritten;
    ptr += nwritten;
  }
  return (n);
}

