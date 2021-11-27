/*
 * gensubcode.c
 * generates subject code entries
 */
#include <stdio.h>


#define STARTING 101
#define ENDING   299

int main(int argc, char **argv)
{
  int i;
  int start, end;

  if (argc == 2)
    start = (int) strtol(argv[1], NULL, 10);
  else
    start = STARTING;

  if (argc == 3)
    end = (int) strtol(argv[2], NULL, 10);
  else
    end = ENDING;

  if (argc > 3) {
    fprintf(stderr, "Usage: %s [START [END]]\n", argv[0]);
    return 0;
  }

  for (i=start; i <= end; i++)
    printf("%d Name of %d\n", i, i);

  return 0;
}
