/*
 * password.c
 */
#include "password.h"

/* create if not exists */
DB *pass_initdb(const char *dbname)
{
  static DB *dbp;
  int ret;
  u_int32_t flags = DB_CREATE; /* open flags */

  /*
   * calling script will create this
   */
  /*
  mkdir(PASSDIR, 0700);
  chmod(PASSDIR, 0700);
  */

  if ((ret = db_create(&dbp, NULL, 0)) != 0) {
    fprintf(stderr, "db_create: %s\n", db_strerror(ret));
    return NULL;
  }
  if ((ret = dbp->open(dbp, NULL, dbname, NULL, DB_BTREE,
	  flags, PASS_CREATE_MODE)) != 0) {
    dbp->err(dbp, ret, "%s", dbname);
    return NULL;
  }
  return dbp;
}

int pass_insertdb(DB *dbp, char *key, password *data, int confirm)
{
  DBT k, d; /* key/data */
  int ret;
  u_int32_t flags;

  memset(&k, 0, sizeof(k));
  memset(&d, 0, sizeof(d));
  k.data = key;
  k.size = strlen(key)+1;
  d.data = data;
  d.size = sizeof(password);

  if (confirm)
    flags = 0;
  else
    flags = DB_NOOVERWRITE;

  switch (ret = dbp->put(dbp, NULL, &k, &d, flags)) {
    case 0:
      printf("db: %s: key stored.\n", (char *)k.data);
      break;
    case DB_KEYEXIST:
      printf("db: %s: key already present, not updating.\n", (char *)k.data);
      break;
    default:
      dbp->err(dbp, ret, "DB->put");
      break;
  }
  return ret;
}


/* delete key/data from dbp */
int pass_deletedb(DB *dbp, char *key)
{
  int ret;
  DBT k;
  u_int32_t flags;

  flags = 0;
  memset(&k, 0, sizeof(k));
  k.data = key;
  k.size = strlen(key)+1;

  if ((ret = dbp->del(dbp, NULL, &k, flags)) == 0)
    printf("db: %s: key deleted.\n", (char *)k.data);
  else {
    dbp->err(dbp, ret, "DB->del");
    return ret;
  }
  return ret;
}


int pass_getdb(DB *dbp, password *buf, const char *key)
{
  int ret;
  DBT k, d;
  u_int32_t flags;

  password *pdb;

  flags = 0;
  memset(&k, 0, sizeof(k));
  memset(&d, 0, sizeof(d));
  k.data = (char *) key;
  k.size = strlen(key)+1;

  if ((ret = dbp->get(dbp, NULL, &k, &d, flags)) == 0) {
//    printf("db: %s: key retrieved\n", (char *)k.data);
    pdb = (password *) d.data;

    memcpy(buf, pdb, d.size);
  }
  else {
    dbp->err(dbp, ret, "DB->get");
    buf = NULL; /* set the buffer to NULL */
  }
  return ret;
}


void pass_printinfo(const password *pdb)
{
  write(1, "pass = ", 7);
  write(1, pdb->pass, pdb->len);
  write(1, "\n", 1);
}


void pass_exitdb(DB *dbp)
{
  dbp->close(dbp, 0);
}

void encrypt_password(const char *pass, password *ppt)
{
//  EVP_MD_CTX mdctx;
//  const EVP_MD *md;
//  int md_len;
  SHA_CTX shactx;


  /*
  OpenSSL_add_all_digests();

  md = EVP_get_digestbyname("sha1");
  */

  memset(ppt->pass, 0, sizeof(password));

  SHA1_Init(&shactx);
  SHA1_Update(&shactx, pass, strlen(pass));
  SHA1_Final(ppt->pass, &shactx);
  ppt->len = SHA_DIGEST_LENGTH;

  /*
  EVP_DigestInit(&mdctx, md);
  EVP_DigestUpdate(&mdctx, pass, strlen(pass));
  EVP_DigestFinal(&mdctx, (unsigned char *) &(ppt->pass), &md_len);
  ppt->len = md_len;
  */

}
