/*
 * inspass.c
 */
#include "password.h"

#define DATE_FORMAT "%Y-%m-%d.%H:%M:%S"

int main(int argc, char **argv)
{
  (void) argc;
  (void) argv;
  DB *dbp;

//  char buf[80];
//  int ret;

  password pwd;
  int disabled;
  int codeval;
  char codestr[5];
  char pass[15];
  char start[20];
  char end[20];
  struct tm tm;

  int confirm = 1;
  int printonly = 0;

  if (argc > 1) {
    if (!strcmp(argv[1], "--print"))
      printonly = 1;
  }

  printf("Information Format:\n"
      "<disabled?> <college_code> <password> <start_time> <end_time>\n"
      "    0/1        4 chars    < 15 chars   19 chars     19 chars\n");

  dbp = pass_initdb(DATABASE);
  if (!dbp)
    return ERR_INIT;

  while (fscanf(stdin, "%d %d %s %s %s", &disabled, &codeval, pass, start, end) == 5) {
//    printf("disabled = %d, codeval = %d, pass = |%s|\n", disabled, codeval, pass);
    sprintf(codestr, "%04d", codeval);

    if (printonly)
      printf("code = |%s|, pass = |%s| start = |%s|, end = |%s|\n", codestr, pass, start, end);
    else {
      encrypt_password(pass, &pwd);
      pwd.disabled = disabled;
      strptime(start, DATE_FORMAT, &tm);
      pwd.start = mktime(&tm);
      strptime(end, DATE_FORMAT, &tm);
      pwd.end = mktime(&tm);
      pass_insertdb(dbp, codestr, &pwd, confirm);
    }
  }

  pass_exitdb(dbp);

  return 0;
}
