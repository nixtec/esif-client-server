/*
 * showregdb.c
 */
#include "regdb.h"

#define ROOTDIR "/var/regform/bdb/Rajshahi"

#ifndef MAXLINE
#define MAXLINE 120
#endif

int display_database(char *database, int raw);
void show_raw_data(void *data, size_t len);
void showdata(void *data, size_t len);

int main(int argc, char **argv)
{
  (void) argc;
  (void) argv;
  char db[200];
  int raw;

  raw = 0;

  if (argc < 3) { /* <prog> <session> <college> */
    printf("Usage: %s <session> <college>\n", argv[0]);
    exit(0);
  }


  sprintf(db, "%s/%s/INSTITUTE_%s.db", ROOTDIR, argv[1], argv[2]);

  while(--argc > 0) {
    if (!strcmp(argv[argc], "--raw"))
      raw = 1;
  }

  display_database(db, raw);

  return 0;
}

int display_database(char *database, int raw)
{
  DB *dbp;
  DBC *dbcp;
  DBT key, data;
  int close_db, close_dbc, ret;
  int total = 0;

  close_db = close_dbc = 0;

  if ((ret = db_create(&dbp, NULL, 0)) != 0) {
    fprintf(stderr, "showregdb: db_create: %s\n", db_strerror(ret));
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
    if (raw)
      show_raw_data(data.data, data.size);
    else
      showdata(data.data, data.size);
    total++;
//    printf("%d, %d\n", key.size, data.size);
  }

  if (ret != DB_NOTFOUND) {
    dbp->err(dbp, ret, "DBcursor->get");
//    goto err;
  }

  fprintf(stderr, "Total %d keys found\n", total);
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
  char *temp;
  size_t i;
  size_t totlen;

  char buf[MAXLINE];
  char pattern[20]; /* SUB_XX pattern */
  int code = 0;
  int line = 0;

  totlen = i = 0;
  temp = data; /* raw data */
  while (totlen < len) {
    line++;
    i = strlen(temp) + 1;
    totlen += i;
//    printf(temp);

    memset(buf, 0, sizeof(buf)); /* clear previous entry */

    switch (line) {
      case 1: /* skip header */
      case 2: /* skip newline */
	break;
      case 3:
	sscanf(temp, "STUDENTS_NAME %[^\r\n]", buf);
	printf("%-40s", buf);
	break;
      case 4:
	sscanf(temp, "FATHERS_NAME %[^\r\n]", buf);
	printf("%-40s", buf);
	break;
      case 5:
	sscanf(temp, "MOTHERS_NAME %[^\r\n]", buf);
	printf("%-40s", buf);
	break;
      case 6:
	sscanf(temp, "GROUP %[^\r\n]", buf);
	if (!strcasecmp(buf, "Science"))
	  printf("0");
	else if (!strcasecmp(buf, "Social Science"))
	  printf("1");
	else if (!strcasecmp(buf, "Arts"))
	  printf("2");
	else if (!strcasecmp(buf, "Commerce"))
	  printf("3");
	else if (!strcasecmp(buf, "Agriculture"))
	  printf("4");
	else if (!strcasecmp(buf, "Home Economics"))
	  printf("5");
	else if (!strcasecmp(buf, "Islamic Studies"))
	  printf("6");
	else if (!strcasecmp(buf, "Music"))
	  printf("7");
	else if (!strcasecmp(buf, "Business"))
	  printf("8");
	else
	  printf(" "); /* unknown */
	break;
      case 7:
	sscanf(temp, "SESSION %[^\r\n]", buf);
	printf("%-4s", buf);
	break;
      case 8:
	sscanf(temp, "BOARD %[^\r\n]", buf);
	if (!strcasecmp(buf, "Dhaka"))
	  printf("0");
	else if (!strcasecmp(buf, "Comilla"))
	  printf("1");
	else if (!strcasecmp(buf, "Rajshahi"))
	  printf("2");
	else if (!strcasecmp(buf, "Jessore"))
	  printf("3");
	else if (!strcasecmp(buf, "Chittagong"))
	  printf("4");
	else if (!strcasecmp(buf, "Barisal"))
	  printf("5");
	else if (!strcasecmp(buf, "Sylhet"))
	  printf("6");
	else
	  printf(" "); /* unknown */
	break;
      case 9:
	sscanf(temp, "CLASS_ROLL %[^\r\n]", buf);
	printf("%-4s", buf);
	break;
      case 10:
	sscanf(temp, "BIRTHDATE %[^\r\n]", buf);
	printf("%11s", buf); /* nn-xxx-nnnn */
	break;
      case 11:
	sscanf(temp, "TOT_LIST %[^\r\n]", buf);
	printf("%-4s", buf);
	break;
      case 12:
	sscanf(temp, "SECTION %[^\r\n]", buf);
	if (!strcasecmp(buf, "A"))
	  printf("1");
	else if (!strcasecmp(buf, "B"))
	  printf("2");
	else if (!strcasecmp(buf, "C"))
	  printf("3");
	else if (!strcasecmp(buf, "D"))
	  printf("4");
	else if (!strcasecmp(buf, "E"))
	  printf("5");
	else if (!strcasecmp(buf, "F"))
	  printf("6");
	else
	  printf("0"); /* unknown */
	break;
      case 13:
	sscanf(temp, "INSTITUTE_CODE %[^\r\n]", buf);
	printf("%-4s", buf);
	break;
      case 14:
	sscanf(temp, "INSTITUTE_NAME %[^\r\n]", buf);
	printf("%-70s", buf); /* 70 chars */
	break;
      case 15:
	sscanf(temp, "PREVIOUS_BOARD %[^\r\n]", buf);
	if (!strcasecmp(buf, "Dhaka"))
	  printf("0");
	else if (!strcasecmp(buf, "Comilla"))
	  printf("1");
	else if (!strcasecmp(buf, "Rajshahi"))
	  printf("2");
	else if (!strcasecmp(buf, "Jessore"))
	  printf("3");
	else if (!strcasecmp(buf, "Chittagong"))
	  printf("4");
	else if (!strcasecmp(buf, "Barisal"))
	  printf("5");
	else if (!strcasecmp(buf, "Sylhet"))
	  printf("6");
	else if (!strcasecmp(buf, "Madrasa"))
	  printf("7");
	else if (!strcasecmp(buf, "Technical"))
	  printf("8");
	else if (!strcasecmp(buf, "Others"))
	  printf("9");
	else
	  printf(" "); /* unknown */
	break;
      case 16:
	sscanf(temp, "PREVIOUS_YEAR %[^\r\n]", buf);
	printf("%-4s", buf);
	break;
      case 17:
	sscanf(temp, "PREVIOUS_ROLL %[^\r\n]", buf);
	printf("%s", buf); /* may be 6 or 10 digits, so not handling */
	break;
      case 18:
	sscanf(temp, "PREVIOUS_REG %[^\r\n]", buf);
	printf("%s", buf); /* may be 6 or 10 digits, so not handling */
	break;
      case 19:
	sscanf(temp, "PREVIOUS_SESSION %[^\r\n]", buf);
	printf("%9s", buf); /* xxxx-xxxx */
	break;
      case 20:
	sscanf(temp, "GENDER %[^\r\n]", buf);
	if (!strcasecmp(buf, "Male"))
	  printf("0");
	else if (!strcasecmp(buf, "Female"))
	  printf("1");
	else
	  printf(" "); /* unknown */
	break;
      case 21:
	sscanf(temp, "RELIGION %[^\r\n]", buf);
	if (!strcasecmp(buf, "Islam"))
	  printf("0");
	else if (!strcasecmp(buf, "Hindu"))
	  printf("1");
	else if (!strcasecmp(buf, "Buddist"))
	  printf("2");
	else if (!strcasecmp(buf, "Christian"))
	  printf("3");
	else if (!strcasecmp(buf, "Others"))
	  printf("4");
	else
	  printf(" ");
	break;
      default:
	code++;
//	fprintf(stderr, temp);
	sprintf(pattern, "SUB_%02d", code);
	strcat(pattern, " %[^\r\n]");
	sscanf(temp, pattern, buf);
	printf("%-3s", buf);
	break;
    }
    temp += i;
  }
  printf("\r\n"); /* for DOS compatibility */
}

void show_raw_data(void *data, size_t len)
{
  char *temp;
  size_t i;
  size_t totlen;

  totlen = i = 0;
  temp = data; /* raw data */
  while (totlen < len) {
    i = strlen(temp) + 1;
    totlen += i;
    printf(temp);
    temp += i;
  }
}
