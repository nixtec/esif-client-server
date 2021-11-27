/*
 * enapass.c
 * enable a college
 */
#include "password.h"

int main(int argc, char **argv)
{
  (void) argc;
  (void) argv;
  DB *dbp;

  int ret;
  password pwd;

  int codeval;
  char codestr[5];

  dbp = pass_initdb(DATABASE);
  if (!dbp)
    return ERR_INIT;

  printf("Information Format:\n"
      "<college_code>\n");

  while (fscanf(stdin, "%d", &codeval) == 1) {
    sprintf(codestr, "%04d", codeval);

    ret = pass_getdb(dbp, &pwd, codestr);
    if (ret < 0) /* really exists */
      continue;
    pwd.disabled = 0; /* enable password */
    pass_insertdb(dbp, codestr, &pwd, 1);
  }

  pass_exitdb(dbp);

  return 0;
}
