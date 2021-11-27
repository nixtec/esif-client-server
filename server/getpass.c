/*
 * getpass.c
 */
#include "password.h"

int main(int argc, char **argv)
{
  (void) argc;
  (void) argv;
  DB *dbp;

  int ret;
  password pwd;
  char buf[20];
  char name[5];

  printf("Enter College Code (4 digits) : ");
  fgets(buf, sizeof(buf), stdin);
  strncpy(name, buf, 4);
  name[4] = '\0';

  if (strlen(name) != 4) {
    printf("Must be 4 digits!\n");
    return 10;
  }

  dbp = pass_initdb(DATABASE);
  if (!dbp)
    return ERR_INIT;

  if ((ret = pass_getdb(dbp, &pwd, name)) < 0)
    return ERR_GET;

  pass_printinfo(&pwd);

  pass_exitdb(dbp);

  return ret;
}
