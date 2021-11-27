/*
 * password.h
 * definitions and includes of all necessary works
 */
#ifndef _PASSWORD_H_
#define _PASSWORD_H_

#include <stdio.h>               /* printf(), etc. */
#include <stdlib.h>              /* exit(), etc. */
#include <string.h>              /* strlen(), etc. */
#include <strings.h>             /* bzero(), etc. */
#include <sys/stat.h>
#include <sys/types.h>           /* standard type definitions */
#include <db.h>                  /* db routines */
#include <unistd.h>
//#include <openssl/evp.h>         /* EVP_MAX_MD_SIZE */
#include <openssl/sha.h>         /* SHA_DIGEST_LENGTH */

/* strptime() requires this for glibc2.x */
#define __USE_XOPEN
#define _XOPEN_SOURCE
#include <time.h>                /* time conversions */

/*
 * int db_create(DB **dbp, DB_ENV *dbenv, u_int32_t flags);
 */

/*
 * sizeof() can compute the size of compile time defined values
 */

typedef struct {
  unsigned char pass[SHA_DIGEST_LENGTH];
  unsigned int len; /* length of the stored password */
  int disabled; /* disabled? */
  time_t start; /* start time of sending form */
  time_t end; /* end time of sending form */
} password;

/* error definitions */
#define ERR_INIT 1
#define ERR_OPEN 2
#define ERR_INS 3
#define ERR_UPD 4
#define ERR_DEL 5
#define ERR_GET 6

#define PASSDIR "var/regform/passwd"
#define DATABASE "var/regform/passwd/pass.db"
#define PASS_CREATE_MODE 0600

/* initialize (or create) database
 * returns handle on success, NULL on failure
 */
DB *pass_initdb(const char *dbname);
/* insert (overwrite if confirm=1) key/data into the database */
int pass_insertdb(DB *dbp, char *key, password *data, int confirm);
/* delete key/value from database */
int pass_deletedb(DB *dbp, char *key);
/* fill buf with data; given key */
int pass_getdb(DB *dbp, password *buf, const char *key);
/* print student information */
void pass_printinfo(const password *pass);
/* exit from database */
void pass_exitdb(DB *dbp);
void encrypt_password(const char *pass, password *ppt);


#endif /* _PASSWORD_H_ */
