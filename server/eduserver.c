/* eduserver.c */
/* server for eSIF Project */

/*
 * pthread_detach(pthread_self()) lets us free resources automatically
 */

/*
 * fflush() is expensive
 * don't call it several times
 */

/*
 * this program will first run as root and then
 * sets its user id to a non-previlized user 'rajreg'
 */

#include "eduserver.h"
#include "support.h"

static DB *passdbp;
static char *passdb;

void *doit(void *arg);
/* lookup user and load password in passbuf */
/* passbuf must be at least EVP_MAX_MD_SIZE bytes long */
int lookup_user(const char *user, password *passbuf);
void init_magic(const char *str, unsigned char *md_value);
void reload_passdb(int signo); /* invoke upon SIGHUP; reload password database */
int getyear(void);
void enable_image(int signo);
void disable_image(int signo);

int accept_binary(int sockfd, size_t size,
    const char *name, void *buffer, size_t bufsize);


static unsigned char magic[SHA_DIGEST_LENGTH]; /* all threads will read it */

/* if client's session doesn't match to this, send their forms in the
 * 'error' directory
 */
//static int current_session; /* current session */

//static int num_of_thread; /* total number of threads created */

static int acceptimage;


int main(int argc, char **argv)
{
  pthread_t tid;
  int listenfd, connfd;
//  socklen_t len;
//  struct sockaddr_in cliaddr; /* dont care client address from here */
  struct sockaddr_in servaddr;
  int i;
  socklen_t clilen;
  int ret;
  uid_t uid;
  gid_t gid;

  const int on = 1; /* SO_REUSEADDR on */
  char *username; /* if $RUNUSER is set, change to that user */

  struct passwd *pwdent;

//  freopen(LOGFILE, "a", stdout);

  (void) argc;
  (void) argv;

  /* options supported:
   * --acceptimage             => clients may send images
   */
  for (i = 1; i < argc; i++) {
    if (!strncmp(argv[i], "--acceptimage", 13))
      acceptimage = 1;
  }

  /* user checking is disabled right now */

  fprintf(stderr, "Checking for RUNUSER environment variable...");
  username = getenv("RUNUSER");
  if (username == NULL) {
    fprintf(stderr, " NOT FOUND. Running as current user\n");
    goto nosetid;
  }

  fprintf(stderr, " FOUND\n");
  /* check if you're root; only root user can seteuid() */
  if (getuid() != 0) {
    fprintf(stderr, "You must be root to run this program!\n");
    fprintf(stderr, "Or remove RUNUSER environment variable\n");
    return 1;
  }

  /* check if USER exists */
  pwdent = getpwnam(username);
  if (!pwdent) {
    fprintf(stderr, "Error : %s: %s\n", username, strerror(errno));
    fprintf(stderr, "This is a security problem to run this program as root!!!\n");
    fprintf(stderr, "First create a user => %s!\n", username);
    fprintf(stderr, "If you don't want this message, remove RUNUSER environment variable\n");
    return 1;
  }

  uid = pwdent->pw_uid;
  gid = pwdent->pw_gid;

  fprintf(stderr, "%s : uid=>%d, gid=>%d\n", username, uid, gid);

  /* now change to the owner to uid */
  fprintf(stderr, "Setting uid to %d\n", uid);
  if (seteuid(uid) != 0) {
    fprintf(stderr, "seteuid: %d: %s\n", uid, strerror(errno));
    return 2;
  }
  /*
  if (setegid(gid) != 0) {
    printf("setegid: %d: %s\n", gid, strerror(errno));
    return 2;
  }
  */
  /* seteuid() complete */
//  /* sete{u,g}id() complete */


nosetid:
  if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "main: socket: %s\n", strerror(errno));
//    fflush(stdout);
    exit(EXIT_FAILURE);
  }


  /* set SO_REUSEADDR so that restarting of server gets success */
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
    fprintf(stderr, "main: setsockopt: %s\n", strerror(errno));
//    fflush(stdout);
//    exit(EXIT_FAILURE);
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
	signal(i, reload_passdb);
	break;
      case SIGINT:
      case SIGTERM:
	signal(i, handle_signal); /* exit program */
	break;
      default:
	signal(i, SIG_IGN);
	break;
    }
  }

  init_magic(MAGIC, magic);

  fprintf(stderr, "main: Waiting for connections ...\n");
//  fflush(stdout);

  if ((passdb = getenv("PASSDB")) == NULL)
    passdb = PASS_DB;

  passdbp = pass_initdb(passdb);
  if (!passdbp)
    return 1;



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

    if ((ret = pthread_create(&tid, NULL, doit, (void *) connfd)) != 0) {
      fprintf(stderr, "main: pthread_create: %s\n", strerror(ret));
//      fflush(stdout);
      close(connfd); /* close socket */
      continue;
    }
//    printf("main: tid = %u\n", (unsigned int) tid);
//    fflush(stdout);
  } /* while() */

  return 0;
}



void *doit(void *arg)
{
  int connfd;
  socklen_t len;
  int ret;
  struct sockaddr_in cli_info;
  char remote_addr[INET_ADDRSTRLEN+1];

  connfd = (int) arg;

  /* free resources on pthread_exit() / return */
  pthread_detach(pthread_self());

  len = sizeof(struct sockaddr_in);

  ret = getpeername(connfd, (struct sockaddr *) &cli_info, &len);
  if (ret < 0) {
    fprintf(stderr, "doit: getpeername: %s\n", strerror(errno));
    close(connfd);
    return NULL;
  }


  if (!inet_ntop(PF_INET, (const void *) &(cli_info.sin_addr), remote_addr, sizeof(remote_addr))) {
    fprintf(stderr, "thread %u: doit: inet_ntop: %s\n", (unsigned int) pthread_self(), strerror(errno));
    close(connfd);
    return (NULL);
  }

  fprintf(stderr, "thread %u: doit: remote_addr = %s, connfd = %d\n", (unsigned int) pthread_self(), remote_addr, connfd);

  manage_client(remote_addr, connfd); /* handle client */

//  printf("\nthread %u: doit: exiting\n", (unsigned int) pthread_self());
//  printf("....................\n");
//  shutdown(connfd, SHUT_WR);
  close(connfd); /* close connection */
  pthread_exit(NULL);
}


/* don't try to close the socket
 * just process the client
 * never try to use local buffers or variables for writing/modifying
 * from within a thread function.
 * always use malloc() for thread-safety
 */
void manage_client(const char *r_addr, int conn_id)
{
  char buffer[MAXLINE];
  password passbuf;

  int college;
  int session;
  char txtdir[MAXDIRNAME];
//  char dbdir[MAXDIRNAME];
  char filename[MAXDIRNAME+MAXFILENAME];
//  char dbname[60];
  char tmpname[MAXFILENAME];
  FILE *stfile = NULL;
  char user[6];
//  int *fd = NULL;
  int i;
  int ret;
  int nwrite;
  char ebuf[5]; /* error buffer (xxx\n\0) */

  struct timeval tv;

  DB *regdbp = NULL; /* database pointer */
  char regdb[50]; /* database name for the college for that session */
  char regkey[20]; /* base filename with .txt stripped */
  int confirm;

  char *regbuf = NULL;
  int regbufsize = 0;

  int imgsize;

  reginfo info;

  memset(buffer, 0, sizeof(buffer)); /* zero fill it */
  memset(&info, 0, sizeof(info));
  memset(&passbuf, 0, sizeof(password));
  memset(&tv, 0, sizeof(tv));


//  printf("thread %u: manage_client: r_addr = %s, conn_id = %d\n", (unsigned int) pthread_self(), r_addr, conn_id);

  /* setting timeout on read */
  /* we can't wait for long for unknown clients
   * but we should wait for long for valid clients
   */
  printf("thread %u: manage_client: r_addr = %s, conn_id = %d : ", (unsigned int) pthread_self(), r_addr, conn_id);
  printf("setting timeout\n");
  tv.tv_sec = TIMEOUT; /* timeout on read from client */
  tv.tv_usec = 0;

  ret = setsockopt(conn_id, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  if (ret != 0) {
    printf("thread %u: manage_client: r_addr = %s, conn_id = %d", (unsigned int) pthread_self(), r_addr, conn_id);
    printf(": setsockopt: SO_RCVTIMEO: %s\n", strerror(errno));
  }
  ret = setsockopt(conn_id, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
  if (ret != 0) {
    printf("thread %u: manage_client: r_addr = %s, conn_id = %d", (unsigned int) pthread_self(), r_addr, conn_id);
    printf(": setsockopt: SO_RCVTIMEO: %s\n", strerror(errno));
  }
  /*  option set   */

  /* client will now send MAGIC */
  /*
   * don't call readline() here; encrypted value may contain embedded newline
   */
  printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
  printf("Waiting for magic ... ");
//  fflush(stdout);
  if ((ret = readn(conn_id, buffer, SHA_DIGEST_LENGTH)) < SHA_DIGEST_LENGTH) {
    if (ret <= 0)
      perror("magic: read");
    else {
      printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
      printf("read %d bytes\n", ret);
      printf("MAGIC required\n");
    }
    printf("[FAILED]\n");
    goto free_resource;
  }
  printf("[  OK  ]\n");
//  printf("ret = %d\n", ret);

  if (memcmp(magic, buffer, SHA_DIGEST_LENGTH)) { /* mismatch */
    printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
    printf("magic mismatch! client not allowed to go further!\n");
    goto free_resource;
  }

  printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
  printf("sending feedback to client\n");

  /* let the client know that */
  if ((ret = writen(conn_id, "500\n", 4)) != 4) {
    printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
    printf("error: write : 500<LF>\n");
    goto free_resource;
  }

  /* don't mixup readline and readn or read functions
   * readline uses buffer and may read more data
   * when you think that all the rest of your lines are <LF> terminated, you
   * can use readline
   */

  /* client will now send USER <username> */
  /*
   * don't call readline() here; encrypted value may contain embedded newline
   */
  printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
  printf("waiting for username ... ");
//  fflush(stdout);
  if ((ret = readn(conn_id, buffer, 10)) < 10) { /* not username */
    if (ret <= 0)
      perror("username: read");
    else {
      printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
      printf("read %d bytes\n", ret);
      printf("Username required\n");
    }
    goto free_resource;
  }
  printf("[  OK  ]\n");
//  printf("ret = %d\n", ret);
  buffer[ret] = '\0'; /* null terminate it */
  if (strncmp(buffer, "USER ", 5)) {
    printf("'USER ' required\n");
    goto free_resource;
  }

  sscanf(buffer, "USER %s", user);


  printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
  printf("looking up user : %s     ", user);
  if (lookup_user(user, &passbuf)) { /* not found */
    printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
    printf("lookup_user: %s: no such user found\n", user);

    sprintf(ebuf, "%03d\n", E_NO_COLLEGE);
    writen(conn_id, ebuf, strlen(ebuf));
    goto free_resource;
  }
  printf("FOUND\n");

  /* username found */
  /* college may be disabled */
  if (passbuf.disabled == 1) {
    printf("DISABLED\n");
    sprintf(ebuf, "%03d\n", E_COLLEGE_DISABLED);
    writen(conn_id, ebuf, strlen(ebuf));
    goto free_resource;
  }

//  printf("passbuf.start = %ld\n", passbuf.start);
//  printf("time() = %ld\n", time(NULL));
  if (passbuf.start > time(NULL)) {
    printf("E_TIME_NOSTART\n");
    sprintf(ebuf, "%03d ", E_TIME_NOSTART);
    strcpy(buffer, ebuf);
    ctime_r(&passbuf.start, buffer+strlen(buffer));
    writen(conn_id, buffer, strlen(buffer));
    goto free_resource;
  }

//  printf("passbuf.end = %ld\n", passbuf.end);
//  printf("time() = %ld\n", time(NULL));
  if (passbuf.end < time(NULL)) {
    printf("E_TIME_EXPIRED\n");
    sprintf(ebuf, "%03d ", E_TIME_EXPIRED);
    strcpy(buffer, ebuf);
    ctime_r(&passbuf.end, buffer+strlen(buffer)); /* ctime_r() places a \n */
    writen(conn_id, buffer, strlen(buffer));
    goto free_resource;
  }

  strcpy(buffer, "500 ");
  ctime_r(&passbuf.start, buffer+strlen(buffer));
  buffer[strlen(buffer)-1] = ' '; /* convert terminating '\n' to ' ' */
  strcat(buffer, "- "); /* client will see this */
  ctime_r(&passbuf.end, buffer+strlen(buffer)); /* ctime_r() automatically places a '\n' */
//  buffer[strlen(buffer)-1] = ' '; /* convert terminating '\n' to ' ' */

  printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
  printf("sending reply for username\n");
//  printf("buffer = %s", buffer);
  nwrite = strlen(buffer);
  if ((ret = writen(conn_id, buffer, nwrite)) != nwrite) {
    goto free_resource;
  }

  /* now check for password */
//  printf("EVP_MAX_MD_SIZE = %d\n", EVP_MAX_MD_SIZE);
  printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
  printf("Waiting for password ... ");
//  fflush(stdout);
  /*
   * don't call readline() here; encrypted value may contain embedded newline
   */
  if ((ret = readn(conn_id, buffer, 5+passbuf.len)) < (int) (5+passbuf.len)) {
    printf("[FAILED]\n");
    if (ret <= 0)
      perror("password: read");
    goto free_resource;
  }
  printf("[  OK  ]\n");
//  printf("ret = %d\n", ret);

  if (strncmp(buffer, "PASS ", 5)) {
    printf("'PASS ' required\n");
    goto free_resource;
  }

  printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
  printf("verifying password ... ");
  if (memcmp(buffer+5, passbuf.pass, passbuf.len)) { /* mismatch */
    printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
    printf("Password mismatch\n");
    sprintf(ebuf, "%03d\n", E_INVALID_PASSWORD);
    writen(conn_id, ebuf, strlen(ebuf));
    goto free_resource;
  }
  printf("[  OK  ]\n");


  /*
   * from now you can use 'readline'
   */


  /* password matched */
  /* let the client know that */
  printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
  printf("sending feedback to client\n");
  if ((ret = writen(conn_id, "500\n", 4)) != 4) {
    printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
    printf("error: send : 500<LF>\n");
    goto free_resource;
  }

//  printf("User authentication successful\n");

  college = (int) strtol(user, NULL, 10);

  /* session is disabled right now
   * client can't send session
   * rather, server will use its own session
   * this will protect from error
   */

  session = getyear();
  sprintf(txtdir, "%s/%04d/", TXTROOT3, session);
  mkdir(txtdir, DIRMODE);
  sprintf(buffer, "INSTITUTE_%04d", college);
  strcat(txtdir, buffer);
  mkdir(txtdir, DIRMODE);
  strcat(txtdir, "/");
  printf("thread %u: manage_client: txtdir = %s\n", (unsigned int) pthread_self(), txtdir);



  sprintf(regdb, "%s/%04d/", BDBROOT3, session);

  mkdir(regdb, DIRMODE);
  sprintf(buffer, "INSTITUTE_%04d", college);
  strcat(regdb, buffer);
  strcat(regdb, ".db");
  printf("thread %u: manage_client: regdb = %s\n", (unsigned int) pthread_self(), regdb);

  if (reg_initdb(&regdbp, regdb) == NULL)
    goto free_resource;


  /* database initialized successfully */
  confirm = 1;


  regbuf = (char *) malloc(MAXINFOSIZE); /* flat memory for reg info */
  if (!regbuf) {
    printf("thread %u: manage_client: malloc: %s\n", (unsigned int) pthread_self(), strerror(errno));
    printf("memory allocation failure\n");
    goto free_resource;
  }

  while (1) {
    ret = readline(conn_id, buffer, MAXLINE);
//    printf("ret = %d\n", ret);
    if (errno == EWOULDBLOCK) { /* timeout */
      printf("thread %u: manage_client: read: %s\n", (unsigned int) pthread_self(), strerror(errno));
      goto free_resource;
    }

    if (!strncmp(buffer, "QUIT\n", 5)) {
      printf("thread %u: manage_client: ***QUIT***\n", (unsigned int) pthread_self());
      goto free_resource;
    }

    /* client now send 'FILENAME filename' */
    if (ret < 9) {
      printf("thread %u: manage_client: filename required\n", (unsigned int) pthread_self());
      goto free_resource;
    }
//    buffer[ret] = '\0'; /* readline will set it automatically */

    if (!strncmp(buffer, "BINARY ", 7)) { /* image transfer */
      if (!acceptimage) { /* image not supported by server */
	printf("E_IMG_NOSUPPORT\n");
	sprintf(ebuf, "%03d\n", E_IMG_NOSUPPORT);
	writen(conn_id, ebuf, strlen(ebuf));
	continue; /* */
      }
      sprintf(ebuf, "%03d\n", SUCCESS); /* Image allowed */
      writen(conn_id, ebuf, strlen(ebuf));

      memset(&imgsize, 0, sizeof(imgsize));
      memset(tmpname, 0, sizeof(tmpname));

      sscanf(buffer, "BINARY %d %[^\r\n]", &imgsize, tmpname);
      if (strlen(tmpname) < 1) {
	sprintf(ebuf, "%03d\n", E_ERROR); /* Image allowed */
	writen(conn_id, ebuf, strlen(ebuf));
	continue;
      }
      strcpy(filename, txtdir);
      strcat(filename, tmpname);
      printf("thread %u: manage_client: image = %s\n", (unsigned int) pthread_self(), filename);

      ret = accept_binary(conn_id, imgsize, filename, regbuf, MAXINFOSIZE); /* use the allocated buffer */
      if (ret == 0) {
	sprintf(ebuf, "%03d\n", SUCCESS); /* Image allowed */
	writen(conn_id, ebuf, strlen(ebuf));
      }
      else {
	sprintf(ebuf, "%03d\n", E_ERROR); /* Image allowed */
	writen(conn_id, ebuf, strlen(ebuf));
      }

      continue; /* continue to next */
    }

    if (strncmp(buffer, "FILENAME ", 9)) {
      printf("thread %u: manage_client: filename required\n", (unsigned int) pthread_self());
      goto free_resource;
    }

//    printf("%s", buffer);

    sscanf(buffer+9, "%[^\r\n]", tmpname); /* obtain filename (may start with space) */

    strcpy(filename, txtdir);
    strcat(filename, tmpname);
    printf("thread %u: manage_client: filename = %s\n", (unsigned int) pthread_self(), filename);


    strncpy(regkey, tmpname, MAXFILENAME);
    regkey[strlen(regkey)-4] = '\0'; /* remove .txt */
    printf("thread %u: manage_client: regkey = %s\n", (unsigned int) pthread_self(), regkey);


//    goto free_resource; /*          <<<<<<<<<<<<<>>>>>>>>>>>>         */
    



    /*
    fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 00600);
    if (fd < 0) {
      printf("thread %u: manage_client: open: %s: %s\n", (unsigned int) pthread_self(), filename, strerror(errno));
      goto free_resource;
    }
    */

    stfile = fopen(filename, "w");
    if (!stfile) {
      printf("thread %u: manage_client: fopen: %s: %s\n", (unsigned int) pthread_self(), filename, strerror(errno));
      goto free_resource;
    }

    /* now get the stream from client */
    i = 0; /* reinitialize line number */
    regbufsize = 0; /* reset buffer index */
    while ((ret = readline(conn_id, buffer, MAXLINE)) > 0) {
      i++;
//      buffer[ret] = '\0'; /* set terminating null */
      if (!strncmp(buffer, ".\n", 2)) {
//	printf("buffer : %s", buffer);
//	fflush(stfile);
	fclose(stfile);
	reg_insertdb(regdbp, regkey, regbuf, regbufsize, confirm);

//	printf("Sending feedback to client\n");
	strcpy(buffer, "500\n");
	if ((ret = writen(conn_id, buffer, strlen(buffer))) != 4) {
	  printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
	  printf("error: send : 500<LF>: %s\n", strerror(errno));
	  goto free_resource;
	}
//	printf("[  OK  ]\n");

	break; /* iterate to next file */
      }
      else if (!strncmp(buffer, "QUIT\n", 5)) {
//	fflush(stfile);
	fclose(stfile);
//	reg_insertdb(regdbp, regkey, info, confirm);

	/*
	if ((ret = send(conn_id, "500 OK", 6)) != 6) {
	  printf("thread %u: manage_client: r_addr = %s, conn_id = %d: ", (unsigned int) pthread_self(), r_addr, conn_id);
	  printf("error: send : 500 OK\n");
	  goto free_resource;
	}
	*/
	goto free_resource;
      }
      /*
      else if (i > TXTLINES) {
	fflush(stfile);
	fclose(stfile);
	reg_insertdb(regdbp, regkey, info, confirm);
	break;
      }
      */
      else {
//	printf("Line %02d: %s", i, buffer);
	if (fputs(buffer, stfile) == EOF) {
	  printf("thread %u: manage_client: fputs: %s\n", (unsigned int) pthread_self(), filename);
//	  fflush(stfile);
	  fclose(stfile);
//	  reg_insertdb(regdbp, regkey, info, confirm);
	  break;
	}
	if (regbufsize >= MAXINFOSIZE) {
	  printf("thread %u: manage_client: regbufsize >= MAXINFOSIZE\n", (unsigned int) pthread_self());
	  break;
	}
	memcpy(regbuf+regbufsize, buffer, ret+1); /* including terminating null */
	regbufsize += ret+1;
      }
    }
    if (ret <= 0) { /* error or closed */
//      fflush(stfile);
      fclose(stfile);
//      reg_insertdb(regdbp, regkey, info, confirm);
      break;
    }
//    */

    /* file sent */
  }

free_resource:
  if (regbuf)
    free(regbuf);
  /* exit from reg database */
  if (regdbp)
    reg_exitdb(regdbp);
}


/* returns 0 if found */
int lookup_user(const char *user, password *passbuf)
{
  if (strlen(user) < 1)
    return 1;

  if (!passdbp)
    return 2;

  if (pass_getdb(passdbp, passbuf, user) < 0) {
    return 3;
  }
  return 0;
}



/* handle signal */
void handle_signal(int signo)
{
  printf("\nhandle_signal: signo = %d\n", signo);
  printf("exiting\n");
  if (passdbp) {
    printf("Closing password database\n");
    pass_exitdb(passdbp); /* close database */
    passdbp = NULL;
  }
  exit(signo);
}

void reload_passdb(int signo)
{
  printf("signal caught : %d\n", signo);
  printf("Reloading password database\n");
  if (passdbp)
    pass_exitdb(passdbp);

  passdbp = pass_initdb(passdb);
  if (!passdbp) {
    printf("FATAL ERROR LOADING PASSWORD DATABASE!\n");
    exit(EXIT_FAILURE);
  }
  return;
}

void enable_image(int signo)
{
  printf("signal caught : %d\n", signo);
  printf("Enabling image\n");

  acceptimage = 1;

  return;
}

void disable_image(int signo)
{
  printf("signal caught : %d\n", signo);
  printf("Enabling image\n");

  acceptimage = 0;

  return;
}


/* md_vlaue must be at least SHA_DIGEST_LEN bytes long */
void init_magic(const char *str, unsigned char *md_value)
{
  SHA_CTX shactx;

  SHA1_Init(&shactx);
  SHA1_Update(&shactx, str, strlen(str));
  SHA1_Final(md_value, &shactx);
}


int getyear(void)
{
  time_t curtime;
  struct tm buf;

  (void) time(&curtime);
  (void) localtime_r(&curtime, &buf);

  return (1900+buf.tm_year);
//  current_session = 1900 + tmp->tm_year;
}

int accept_binary(int sockfd, size_t size,
    const char *name, void *buffer, size_t bufsize)
{
  size_t nleft;
  ssize_t nread;
  char *ptr;
  size_t i;

  int fd;
  int flags;

  ptr = buffer;
  nleft = size;

  flags = O_WRONLY|O_CREAT|O_TRUNC;

  if ((fd = open(name, flags, FILEMODE)) == -1) {
    fprintf(stderr, "accept_binary: open: %s: %s\n", name, strerror(errno));
    return fd;
  }

  i = 0;
  while (nleft > 0) {
    if (nleft > bufsize)
      i = bufsize; /* read buffer size */
    if ((nread = read(sockfd, ptr, i)) < 0) {
      if (errno == EINTR)
	nread = 0;
      else {
	close(fd);
	return (-1);
      }
    } else if (nread == 0)
      break; /* EOF */

    if (write(fd, ptr, nread) != nread) { /* a single byte of data loss is not allowed */
      fprintf(stderr, "accept_binary: write: %s: %s\n", name, strerror(errno));
      break;
    }

    nleft -= nread;
  }

  close(fd); /* close file */

  return (nleft); /* 0 on success */
}
