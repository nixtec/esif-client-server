/*
 * delpass.c
 */
#include "password.h"

int main(int argc, char **argv)
{
  (void) argc;
  (void) argv;
  DB *dbp;

//  int ret;

//  password pwd;
//  int disabled;
  int codeval;
  char codestr[5];
//  char pass[15];

//  int confirm = 1;

  printf("Information Format:\n"
      "<college_code>\n"
      "  4 chars\n");

  dbp = pass_initdb(DATABASE);
  if (!dbp)
    return ERR_INIT;

  while (fscanf(stdin, "%d", &codeval) == 1) {
    sprintf(codestr, "%04d", codeval);

    pass_deletedb(dbp, codestr);

  }

  pass_exitdb(dbp);

  return 0;
}
