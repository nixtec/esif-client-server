/*
 * regdb.h
 * definitions and includes of all necessary works
 */
#ifndef _REGDB_H_
#define _REGDB_H_

#include <stdio.h>               /* printf(), etc. */
#include <stdlib.h>              /* exit(), etc. */
#include <string.h>              /* strlen(), etc. */
#include <strings.h>             /* bzero(), etc. */
#include <sys/types.h>           /* standard type definitions */
#include <db.h>                  /* db routines */

//#include <openssl/evp.h>         /* EVP_MAX_MD_SIZE */

#ifndef TXTLINES
#define TXTLINES 39
#endif

#ifndef REG_CREATE_MODE
#define REG_CREATE_MODE 0640
#endif


/*
 * int db_create(DB **dbp, DB_ENV *dbenv, u_int32_t flags);
 */

/*
 * sizeof() can compute the size of compile time defined values
 */

typedef struct {
  char *data[TXTLINES];
} reginfo;

/* error definitions */
#ifndef ERR_INIT
#define ERR_INIT 1
#endif
#ifndef ERR_OPEN
#define ERR_OPEN 2
#endif
#ifndef ERR_INS
#define ERR_INS 3
#endif
#ifndef ERR_UPD
#define ERR_UPD 4
#endif
#ifndef ERR_DEL
#define ERR_DEL 5
#endif
#ifndef ERR_GET
#define ERR_GET 6
#endif

/* initialize (or create) database
 * returns handle on success, NULL on failure
 */
DB *reg_initdb(DB **dbp, const char *dbname);
/* insert (overwrite if confirm=1) key/data into the database */
int reg_insertdb(DB *dbp, char *key, void *data, size_t len, int confirm);
/* delete key/value from database */
int reg_deletedb(DB *dbp, char *key);
/* fill buf with data; given key */
int reg_getdb(DB *dbp, reginfo *buf, const char *key);
/* print student information */
void reg_printinfo(const reginfo *reg);
/* exit from database */
void reg_exitdb(DB *dbp);

#endif /* _REGDB_H */
