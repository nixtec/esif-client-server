/*
 * regdb.c
 */
#include "regdb.h"

DB *reg_initdb(DB **dbpp, const char *dbname)
{
  int ret;
  DB *dbp;

  if ((ret = db_create(dbpp, NULL, 0)) != 0) {
    fprintf(stderr, "db_create: %s\n", db_strerror(ret));
    return (NULL);
  }
  dbp = *dbpp;

  if ((ret = dbp->open(dbp, NULL, dbname, NULL, DB_BTREE, DB_CREATE, REG_CREATE_MODE)) != 0) {
    dbp->err(dbp, ret, "%s", dbname);
    return (NULL);
  }

  return (*dbpp);
}

/* 'data' must be pre filled up */
/* len => size of data */
int reg_insertdb(DB *dbp, char *key, void *data, size_t len, int confirm)
{
  DBT k, d; /* key/data */
  int ret;
  u_int32_t flags;

  /* now store the data */
  memset(&k, 0, sizeof(DBT));
  memset(&d, 0, sizeof(DBT));
  k.data = key;
  k.size = strlen(key)+1;
  d.data = data;
  d.size = len;


  if (confirm)
    flags = 0;
  else
    flags = DB_NOOVERWRITE;

  switch (ret = dbp->put(dbp, NULL, &k, &d, flags)) {
    case 0:
//      printf("db: %s: key stored.\n", (char *) k.data);
      break;
    case DB_KEYEXIST:
      printf("db: %s: key already present, not updating.\n", (char *) k.data);
      break;
    default:
      dbp->err(dbp, ret, "DB->put");
      break;
  }

  return ret;
}


/* delete key/data from dbp */
int reg_deletedb(DB *dbp, char *key)
{
  int ret;
  DBT k;
  u_int32_t flags;

  flags = 0;

  memset(&k, 0, sizeof(DBT));
  k.data = key;
  k.size = strlen(key)+1;

  if ((ret = dbp->del(dbp, NULL, &k, flags)) == 0)
    printf("db: %s: key deleted.\n", (char *) k.data);
  else {
    dbp->err(dbp, ret, "DB->del");
    return 1;
  }
  return (0);
}


/*
 * any NULL line will be allocated by this function
 * caller must free data when no longer necessary
 * */
int reg_getdb(DB *dbp, reginfo *buf, const char *key)
{
  int ret;
  DBT k, d;
  u_int32_t flags;
  char *buffer = NULL; /* data buffer (don't free it) */
  int i;
  int len;
  char *temp;

  flags = 0;
  memset(&k, 0, sizeof(DBT));
  memset(&d, 0, sizeof(DBT));
  k.data = (char *) key;
  k.size = strlen(key)+1;

  /* get information stored in database */
  if ((ret = dbp->get(dbp, NULL, &k, &d, flags)) == 0) {
    printf("db: %s: key retrieved\n", (char *) k.data);
    temp = buffer = (char *) d.data; /* raw data */

    len = 0;
    /* now fill the buffer */
    for (i = 0; i < TXTLINES; i++) {
      len = strlen(temp) + 1; /* including null */
      if (buf->data[i] == NULL) /* NULL */
	buf->data[i] = (char *) malloc(len);
      memcpy(buf->data[i], temp, len);
      temp += len; /* forward the pointer */
    }
  }
  else {
    dbp->err(dbp, ret, "DB->get");
    buf = NULL; /* set the buffer to NULL */
    return 1;
  }
  return 0;
}


void reg_printinfo(const reginfo *rdb)
{
  int i;

  for (i = 0; i < TXTLINES; i++)
    printf("Line %02d: %s\n", i+1, rdb->data[i]);
}


void reg_exitdb(DB *dbp)
{
  dbp->close(dbp, 0);
}
