#include <stdio.h>
#include <string.h>


int main(int argc, char **argv)
{
  int disabled;
  char session[5];
  char college[5];
  char pass[15];

  int i;

  (void) argc;
  (void) argv;

  disabled = 0;
  sprintf(session, "%04d", 2005);

  for (i = 0; i < 8000; i++) {
    sprintf(college, "%04d", 1000+i);
    strcpy(pass, session);
    strcat(pass, college);
    printf("%d %s %s %s-03-12.18:31:01 %s-04-15.18:31:01\n",
	disabled, college, pass, session, session);
  }

  return 0;
}
