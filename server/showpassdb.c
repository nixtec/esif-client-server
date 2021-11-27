/*
 * showpassdb.c
 */
#include "password.h"

int display_database(char *database);
void show_raw_data(void *data, size_t len);
void showdata(void *data, size_t len);

int main(int argc, char **argv)
{
  (void) argc;
  (void) argv;
  DB *dbp;

  int ret;
  password pwd;
//  char buf[20];
  char name[5];

  if (argc == 1) { /* no paramaters supplied */
    goto display_all;
//    printf("%s <college_code> [...]\n", argv[0]);
//    exit(0);
  }

  dbp = pass_initdb(DATABASE);
  if (!dbp)
    return ERR_INIT;

  while (--argc > 0) {
    memset(name, 0, sizeof(name));
    strncpy(name, argv[argc], 5);
    if ((ret = pass_getdb(dbp, &pwd, name)) < 0)
      continue;

    printf("%s (%s)\n\tstart: %s\tend  : %s\n", name, pwd.disabled? "disabled" : "enabled", ctime(&pwd.start), ctime(&pwd.end));
  }
  goto go_out;

display_all:
  display_database(DATABASE);
//  pass_printinfo(&pwd);

//  pass_exitdb(dbp);

go_out:
  return 0;
}

int display_database(char *database)
{
  DB *dbp;
  DBC *dbcp;
  DBT key, data;
  int close_db, close_dbc, ret;
  int total = 0;

  close_db = close_dbc = 0;

  if ((ret = db_create(&dbp, NULL, 0)) != 0) {
    fprintf(stderr, "showpassdb: db_create: %s\n", db_strerror(ret));
    return 1;
  }

  /* open the database */
  if ((ret = dbp->open(dbp, NULL, database, NULL, DB_UNKNOWN, DB_RDONLY, 0)) != 0) {
    dbp->err(dbp, ret, "%s: DB->open", database);
    goto err;
  }

  /* acquire a cursor for the database */
//  printf("Acquiring cursor\n");
  if ((ret = dbp->cursor(dbp, NULL, &dbcp, 0)) != 0) {
    dbp->err(dbp, ret, "DB->cursor");
    goto err;
  }

  /* initialize the key/data pair */
  memset(&key, 0, sizeof(key));
  memset(&data, 0, sizeof(data));

//  printf("Getting info :\n");
  /* walk through the database */
  while ((ret = dbcp->c_get(dbcp, &key, &data, DB_NEXT)) == 0) {
    printf("%s :\r\n", (char *) key.data);
    showdata(data.data, data.size);
    printf("\r\n");
    total++;
//    printf("%d, %d\n", key.size, data.size);
  }

  if (ret != DB_NOTFOUND) {
    dbp->err(dbp, ret, "DBcursor->get");
//    goto err;
  }

  fprintf(stderr, "Total %d keys found\r\n", total);
err:
//  printf("Closing database\n");
  if (close_dbc && (ret = dbcp->c_close(dbcp)) != 0)
    dbp->err(dbp, ret, "DBcursor->close");
  if (close_db && (ret = dbp->close(dbp, 0)) != 0)
    fprintf(stderr, "display_database: DB->close: %s\n", db_strerror(ret));

  return 0;
}

void showdata(void *data, size_t len)
{
  password *pwd;
  (void) len;

  pwd = data;

  printf("\t%s\n", (pwd->disabled) ? "disabled" : "enabled");
  printf("\tStart : %s", ctime(&pwd->start)); /* ctime() places a \n automatically */
  printf("\tEnd   : %s", ctime(&pwd->end));
}
