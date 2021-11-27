/* educlient.c */
#include "educlient.h"
#include <openssl/evp.h>


int authenticate(const char *username, const char *pass);

static int sockfd;

/* the main routine for client program */
int main (int argc, char **argv)
{
  struct sockaddr_in servaddr; /* server socket address structure */
  char ipaddr[16];             /* server ip address */
  char buffer[MAXLINE];

  int val;
  int ret;

  
  
  if (argc < 2) /* ip address not supplied */
    strcpy (ipaddr, "127.0.0.1");
  else /* assume ipaddress supplied */
    strcpy (ipaddr, argv[1]); /* save the ip address for further use */

  memset(&servaddr, 0, sizeof (servaddr));
  servaddr.sin_family = PF_INET;        /* socket address family to use */
  servaddr.sin_port = htons (SERV_PORT); /* port of the server */

  if (inet_pton (PF_INET, ipaddr, &servaddr.sin_addr) <= 0) {
    printf ("inet_pton: %s\n", strerror (errno));
    close (sockfd);
    exit (EXIT_FAILURE); /* terminate the application (may be dummy) */
  }

  /* a valid address supplied,
   * so creating socket for this connection is meaningful */
  if((sockfd = socket (PF_INET, SOCK_STREAM, 0)) < 0) {
    printf ("socket: %s\n", strerror (errno));
    exit (EXIT_FAILURE);
  }

  signal (SIGHUP, handle_signal); /* hangup signal */
  signal (SIGINT, handle_signal); /* interrupt signal */
  signal (SIGQUIT, handle_signal); /* quit signal */
  signal (SIGTERM, handle_signal); /* termination signal */

  printf ("Connecting to server : %-30s", ipaddr);
  if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1) {
    printf ("[FAILED]\nconnect: %s, errno = ", strerror (errno));
    switch(errno) {
      case EBADF:
	printf("EBADF\n");
	break;
      case EFAULT:
	printf("EFAULT\n");
	break;
      case ENOTSOCK:
	printf("ENOTSOCK\n");
	break;
      case EISCONN:
	printf("ENOTSOCK\n");
	break;
      case ECONNREFUSED:
	printf("ECONNREFUSED\n");
	break;
      case ETIMEDOUT:
	printf("ETIMEDOUT\n");
	break;
      case ENETUNREACH:
	printf("ENETUNREACH\n");
	break;
      case EADDRINUSE:
	printf("EADDRINUSE\n");
	break;
      case EAGAIN:
	printf("EAGAIN\n");
	break;
      default:
	printf("Unknown\n");
    }
    close (sockfd);
    exit (EXIT_FAILURE);
  }

  printf("[  OK  ]\n");

  if (authenticate("0110", "ttippp")) {
    printf("Authentication failure\n");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  val = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, val | O_NONBLOCK);

  while (1) {
    ret = read(sockfd, buffer, MAXLINE-1);
    if (ret == 0) { /* EOF */
      printf("Server closed socket\n");
      break;
    }
    else if ((ret < 0) && (errno != EAGAIN)) { /* error */
      printf("error: read: %s\n", strerror(errno));
      break;
    }
    else if (ret > 0) { /* data available */
      buffer[ret] = '\0';
      printf("SERVER: %s\n", buffer);
    }

    if (fgets(buffer, sizeof(buffer), stdin)) {
      if (ret == 0) {
	printf("Server closed socket\n");
	break;
      }
      if (write(sockfd, buffer, strlen(buffer)) == -1) {
	printf("error: write: %s\n", strerror(errno));
	break;
      }
    }
    else
      break;
  }

  close(sockfd);
  printf ("Client terminating...\n");
  exit (EXIT_FAILURE);
}

/* the signal handler function */
/* client logout, OS sending signals to the client */
void handle_signal(int signo)
{
  printf("Signal %d handled\n", signo);
  close (sockfd);
  exit (EXIT_FAILURE); // terminate application upon logout
}


/* pass is simple text right now
 * but will be sha1 encrypted before sending to server
 */
int authenticate(const char *username, const char *pass)
{
  char buffer[80];
  EVP_MD_CTX mdctx; /* message digest context */
  const EVP_MD *md;
  static unsigned char md_value[EVP_MAX_MD_SIZE+1];
  int md_len;

  int nwrite;
  int ret;

  printf("EVP_MAX_MD_SIZE = %d\n", EVP_MAX_MD_SIZE);

  if (!username || strlen(username) < 4) /* college code */
    return 1;
  if (!pass || strlen(pass) < 6) /* must be at least 6 characters */
    return 2;

  /* first send username */
  sprintf(buffer, "USER %s\n", username);
  nwrite = strlen(buffer)+1;
  printf("Sending username ... ");
  if ((ret = write(sockfd, buffer, nwrite)) != nwrite) {
    printf("[FAILED]\n");
    return 1;
  }
  printf("[OK]\n");

  /* initializing password */
  printf("Initializing digest\n");
  OpenSSL_add_all_digests();

  md = EVP_get_digestbyname(DIGEST);

  if (!md) {
    printf("Unknown message digest %s\n", DIGEST);
    return 3;
  }

  memset(md_value, 0, sizeof(md_value));

  EVP_DigestInit(&mdctx, md);
  EVP_DigestUpdate(&mdctx, pass, strlen(pass));
  EVP_DigestFinal(&mdctx, md_value, &md_len);
  printf("Digest finalization complete\n");

  printf("Password is %d bytes long\n", md_len);
  memcpy(buffer, "PASS ", 6);
  memcpy(buffer+5, md_value, md_len);
  *(buffer+5+md_len) = '\0';

  sleep(3);
  nwrite = 5+md_len+1;
  printf("buff = %s\n", buffer);
  printf("len = %d\n", nwrite);
  printf("Writing password to socket ... ");
  if ((ret = write(sockfd, buffer, nwrite)) != nwrite) {
    printf("[FAILED]\n");
    return 1;
  }
  printf("[  OK  ]\n");
  printf("Waiting for reply ...");
  if ((ret = read(sockfd, buffer, MAXLINE)) < 3) {
    printf("[FAILED]\n");
    return 1;
  }
  printf("[  OK  ]\n");
  if (!strncmp(buffer, "500", 3)) {
    printf("Authentication Successful\n");
    return 0;
  }
  else {
    printf("Authentication failure\n");
    return 1;
  }

  return 0;
}
