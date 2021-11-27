#include "includes.h"
#include "myfuncs.h"
#include "editor.h"
#include "support.h"
#include "callbacks.h"

/* socket timeout */
#ifndef TIMEOUT
#define TIMEOUT 60
#endif


#define E_ERROR 300
#define E_NO_COLLEGE 301
#define E_COLLEGE_DISABLED 302
#define E_INVALID_PASSWORD 303
#define E_TIME_NOSTART 304 /* time not yet started */
#define E_TIME_EXPIRED 305 /* time expired (not allowed to fill form) */

#define E_IMG_NOSUPPORT 310


#ifdef _WIN32
char *convert_win_to_posix(char *path);
#endif



//#ifdef _WIN32
//typedef int ssize_t;
//#endif

#ifdef _WIN32
static ssize_t my_read(SOCKET fd, char *ptr);
#else
static ssize_t my_read(int fd, char *ptr);
#endif

#ifdef _WIN32
ssize_t readline(SOCKET fd, void *vptr, size_t maxlen);
#else
ssize_t readline(int fd, void *vptr, size_t maxlen);
#endif

#ifdef _WIN32
ssize_t readn(SOCKET fd, void *vptr, size_t n);
#else
ssize_t readn(int fd, void *vptr, size_t n);
#endif

#ifdef _WIN32
ssize_t writen(SOCKET fd, const void *vptr, size_t n);
#else
ssize_t writen(int fd, const void *vptr, size_t n);
#endif

#ifdef _WIN32
void init_winsock(void);
static SOCKET sockfd;
static int winsock_ok = 0; /* winsock.dll initialization happened ? */
#else
static int sockfd; /* connection handle */
#endif

static char pass[20]; /* college password */
//static char login[5];
//static char *formdir = NULL;
static char website[40];
static char webaddr[20];
static int lookup_ok; /* if TRUE, new lookup will not be performed */

int all_col_code;
char all_colcode[5]; /* string value (4 digits) */
char all_col_name[71];
char all_session[5];

char error_msg[600];

int ignore_fa_name;
int ignore_mo_name;
int ignore_birthdate;
int ignore_totlist;
int ignore_prev_year;
int ignore_prev_roll;
int ignore_prev_reg;
int ignore_prev_sess;
int ignore_photo;

int is_ssc; /* ssc or hsc form */

static int submit_cancelled; /* cancelled submit */

static size_t prevlen; /* previous length of the session entry */

/* group => set of subject codes valid for that group */
const char *groupfiles[][3] = {
  { "Science", "sub_sc.txt", "sub_4th_sc.txt" },
  { "Social Science", "sub_so_sc.txt", "sub_4th_so_sc.txt" },
  { "Arts", "sub_arts.txt", "sub_4th_arts.txt" },
  { "Commerce", "sub_com.txt", "sub_4th_com.txt" },
  { "Agriculture", "sub_agri.txt", "sub_4th_agri.txt" },
  { "Home Economics", "sub_home_eco.txt", "sub_4th_home_eco.txt" },
  { "Islamic Studies", "sub_is_st.txt", "sub_4th_is_st.txt" },
  { "Music", "sub_music.txt", "sub_4th_music.txt" },
  { "Business", "sub_busi.txt", "sub_4th_busi.txt" },
  { NULL, "" } /* functions will not use this filename */
};

/* message window need not be created every time */
static GtkWidget *msgwin; /* message window pointer */
/* label should be rewritten when a new things happens */
static GtkWidget *msglabel; /* label widget for message */

static GtkWidget *pass_window; /* password window */
static GtkWidget *online_transfer_win;

/* subject hash table pointer for faster lookup of
 * subject code => subject name
 * should be initialized only once
 */
static GHashTable *subhtable;
static GHashTable *pairhtable; /* subject code pair matching */

static GHashTable *colhtable; /* college code->name hash table */

//static GtkWidget *photo_window;
static GtkWidget *load_photo;
static GtkWidget *options_window;


void fix_info_path(void)
{
  strcpy(infodirname, ROOTDIR); /* /home/ayub */
  MKDIR(infodirname, 0755);
  strcat(infodirname, "/");
  strcat(infodirname, BOARD); /* /home/ayub/Rajshahi */
  MKDIR(infodirname, 0755);
  strcat(infodirname, "/");
  strcat(infodirname, all_session); /* /home/ayub/Rajshahi/2005 */
  MKDIR(infodirname, 0755);
  strcat(infodirname, "/");
  strcat(infodirname, "INSTITUTE_");
  strcat(infodirname, all_colcode); /* /home/ayub/Rajshahi/2005/INSTITUTE_XXXX */
  MKDIR(infodirname, 0755);
  strcat(infodirname, "/");

  PRINTF("fix_info_path: infodirname = %s\n", infodirname);
}

/* data is a widget within the window */
/* if clear == TRUE, clear some entries */
int verify_form(gpointer data, gboolean clear)
{
  GtkWidget *ptr = NULL; /* common pointer */
  FILE *fp;

//  int res;
  int err;

  int i;

//  int numsub = 0; /* number of subject code entries filled up */

//  char year_str[6];
//  time_t curtime;

//  char *logname = "log.txt";

//  freopen(logname, "w", stdout);


  /*
  curtime = time(NULL);
  strftime(year_str, 5, "%Y", localtime(&curtime));
  strcat(fulldirname, year_str);
  strcat(fulldirname, "/");
  PRINTF("fulldirname = %s\n", fulldirname);
  if ((res = MKDIR(fulldirname, 0755)) != 0) {
    PRINTF("mkdir: %s: %s\n", fulldirname, strerror(errno));
  }
  */



  PRINTF("\n**verify_form**\n\n");

  /* 1. student's name */
  char *student_name = NULL;
  PRINTF("entry2->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "entry2");
  if (ptr != NULL) {
    student_name = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
    PRINTF("Student's Name : %s\n", student_name);
  }

  /* 2. father's name */
  char *father_name = NULL;
  PRINTF("entry3->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "entry3");
  if (ptr != NULL) {
    father_name = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
    PRINTF("Father's Name : %s\n", father_name);
  }

  /* 3. mother's name */
  char *mother_name = NULL;
  PRINTF("entry4->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "entry4");
  if (ptr != NULL) {
    mother_name = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
    PRINTF("Mother's Name : %s\n", mother_name);
  }


  /* 4. group */
  char *group = NULL;
  PRINTF("combo_entry1->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "combo_entry1");
  if (ptr != NULL) {
    group = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
    PRINTF("Group : %s\n", group);
  }

  /* 5. Session */
  char *session = NULL;
  PRINTF("entry->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "entry5");
  if (ptr != NULL) {
    session = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
  }
  PRINTF("Session : %s\n", session);

  /* 6. board */
  char *board = NULL;
  PRINTF("combo_entry2->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "combo_entry2");
  if (ptr != NULL) {
    board = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
    PRINTF("Board : %s\n", board);
  }

  /* 7. Class Roll Number */
  char *class_roll = NULL;
  int classroll;
  PRINTF("entry6->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "entry6");
  if (ptr != NULL) {
    class_roll = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
  }
  PRINTF("Class Roll Number : %s\n", class_roll);


  /* 8. Date of Birth */
  char *day_of_birth = NULL;
  PRINTF("combo_entry3->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "combo_entry3");
  if (ptr != NULL) {
    day_of_birth = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
  }
  char *month_of_birth = NULL;
  PRINTF("combo_entry4->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "combo_entry4");
  if (ptr != NULL) {
    month_of_birth = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
  }
  char *year_of_birth = NULL;
  PRINTF("combo_entry5->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "combo_entry5");
  if (ptr != NULL) {
    year_of_birth = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
  }
  char birthdate[12];
  sprintf(birthdate, "%s-%s-%s", day_of_birth, month_of_birth, year_of_birth);
  PRINTF("Birth Date : %s\n", birthdate);

  /* 9. TOT List Serial Number */
  char *tot_list = NULL;
  PRINTF("entry7->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "entry7");
  if (ptr != NULL) {
    tot_list = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
  }
  PRINTF("TOT List Serial Number : %s\n", tot_list);


  /* 10. section */
  char *section = NULL;
  PRINTF("combo_entry6->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "combo_entry6");
  if (ptr != NULL) {
    section = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
    PRINTF("Section : %s\n", section);
  }

  /* 11. college code */
  char *college_code = NULL;
  char *college_name = NULL;
  PRINTF("entry8->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "entry8");
  if (ptr != NULL) {
    college_code = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
  }
  PRINTF("College Code : %s\n", college_code);

  PRINTF("entry9->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "entry9");
  if (ptr != NULL) {
    college_name = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
  }
  PRINTF("College Name : %s\n", college_name);


  /* 12. Previous Information */
  /* previous board */
  char *previous_board = NULL;
  PRINTF("combo_entry7->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "combo_entry7");
  if (ptr != NULL) {
    previous_board = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
    PRINTF("Previous Board : %s\n", previous_board);
  }

  /* previous year */
  char *previous_year = NULL;
  PRINTF("entry10->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "entry10");
  if (ptr != NULL) {
    previous_year = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
    PRINTF("Previous Year : %s\n", previous_year);
  }

  /* previous roll number */
  char *previous_roll = NULL;
  PRINTF("entry11->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "entry11");
  if (ptr != NULL) {
    previous_roll = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
    PRINTF("Previous Roll Number : %s\n", previous_roll);
  }

  /* previous registration number */
  char *previous_reg = NULL;
  PRINTF("entry12->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "entry12");
  if (ptr != NULL) {
    previous_reg = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
    PRINTF("Previous Registration Number : %s\n", previous_reg);
  }

  /* previous session start */
  char *previous_sess_start = NULL;
  PRINTF("entry13->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "entry13");
  if (ptr != NULL) {
    previous_sess_start = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
  }
  /* previous session end */
  char *previous_sess_end = NULL;
  PRINTF("entry14->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "entry14");
  if (ptr != NULL) {
    previous_sess_end = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
  }
  PRINTF("Previous Session : %s-%s\n", previous_sess_start, previous_sess_end);


  /* 13. gender */
  char *gender = NULL;
  PRINTF("combo_entry8->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "combo_entry8");
  if (ptr != NULL) {
    gender = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
    PRINTF("Group : %s\n", gender);
  }

  /* 14. religion */
  char *religion = NULL;
  PRINTF("combo_entry9->lookup\n");
  ptr = lookup_widget(GTK_WIDGET(data), "combo_entry9");
  if (ptr != NULL) {
    religion = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
    PRINTF("Religion : %s\n", religion);
  }


  /* subject codes */
  char *subs[NUM_TOTAL_SUB+1]; /* the last one is for termination */
  memset(subs, 0, sizeof(subs)); /* make all NULL */

  char entryname[10];

  int subidx = 0;

  for (subidx=0; subidx<NUM_TOTAL_SUB; subidx++) { /* entry15-entry29 */
    sprintf(entryname, "entry%d", subidx+15);
    PRINTF("%s->lookup\n", entryname);
    ptr = lookup_widget(GTK_WIDGET(data), entryname);
    if (ptr != NULL) {
      subs[subidx] = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
      PRINTF("Subject %d : %s\n", subidx, subs[subidx]);
    }
  }




  /* check all values here */
  /* assuming all values are ok, we can proceed to writing to disk */

  PRINTF("Verifying student name\n");
  if (verify_name(student_name)) {
    PRINTF("***ERROR*** in student name\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry2");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

//    show_message("Error! Fix Student's Name");
    err = E_STUDENT_NAME;
    goto error;
  }


  if (ignore_fa_name)
    goto no_fa_name;

  PRINTF("Verifying father name\n");
  if (verify_name(father_name)) {
    PRINTF("***ERROR*** in father name\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry3");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

//    show_message("Error! Fix Father's Name");
    err = E_FATHERS_NAME;
    goto error;
  }

no_fa_name:

  if (ignore_mo_name)
    goto no_mo_name;

  PRINTF("Verifying mother name\n");
  if (verify_name(mother_name)) {
    PRINTF("***ERROR*** in mother name\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry4");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

//    show_message("Error! Fix Mother's Name");
    err = E_MOTHERS_NAME;
    goto error;
  }

no_mo_name:
  PRINTF("Verifying session\n");
  if (verify_number(session)) {
    PRINTF("***ERROR*** in session\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry5");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

//    show_message("Error! Fix Session");
    err = E_SESSION;
    goto error;
  }
    
  PRINTF("Verifying class roll no\n");
  if ((atoi(class_roll) < 1) || verify_number(class_roll)) {
    PRINTF("***ERROR*** in class roll no\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry6");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

//    show_message("Error! Fix Class Roll");
    err = E_CLASS_ROLL;
    goto error;
  }

  if (ignore_birthdate)
    goto no_birthdate;

  PRINTF("Verifying date of birth\n");

  if ((strlen(year_of_birth) < 4) || verify_number(year_of_birth)) {

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "combo_entry5");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

    err = E_BIRTH_DATE;
    goto error;
  }

  int dd, yyyy;

  dd = atoi(day_of_birth);
  yyyy = atoi(year_of_birth);
  if (verify_date(dd, month_of_birth, yyyy)) {
    PRINTF("***ERROR*** in date of birth\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "combo_entry5");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

    err = E_BIRTH_DATE;
    goto error;
  }
  /* do extra checking of birth year */
  if ((yyyy > atoi(all_session)-10) || (yyyy <= atoi(all_session)-30)) { /* age must be between 10 and 30 */
    PRINTF("***ERROR*** in date of birth\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "combo_entry5");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

    err = E_BIRTH_DATE;
    goto error;
  }

no_birthdate:

  if (ignore_totlist)
    goto no_totlist;

  PRINTF("Verifying tot list\n");
  if ((strlen(tot_list) < 1) || verify_number(tot_list)) {
    PRINTF("***ERROR*** in tot list\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry7");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

    err = E_TOT_LIST;
    goto error;
  }

no_totlist:

  PRINTF("Verifying college code\n");
  if (verify_number(college_code)) {
    PRINTF("***ERROR*** in college code\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry8");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

//    show_message("Error!!! Fix College Code");
    err = E_COLLEGE_CODE;
    goto error;
  }

  if (is_ssc)
    goto ssc; /* don't verify previous info for S. S. C */

  if (ignore_prev_year)
    goto no_prev_year;

  PRINTF("Verifying previous year\n");
  if ((strlen(previous_year) < 4) || verify_number(previous_year)) {
    PRINTF("***ERROR*** in previous year\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry10");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

    err = E_PREVIOUS_YEAR;
    goto error;
  }
  /* do some extra checking */
  if (atoi(previous_year) >= atoi(all_session)) {
    PRINTF("***ERROR*** in previous year\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry10");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

    err = E_PREVIOUS_YEAR;
    goto error;
  }

no_prev_year:

  if (ignore_prev_roll)
    goto no_prev_roll;

  PRINTF("Verifying previous roll\n");
  if ((strlen(previous_roll) < 6) || verify_number(previous_roll)) {
    PRINTF("***ERROR*** in previous roll\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry11");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

    err = E_PREVIOUS_ROLL;
    goto error;
  }

no_prev_roll:

  if (ignore_prev_reg)
    goto no_prev_reg;

  PRINTF("Verifying previous reg\n");
  if ((strlen(previous_reg) < 6) || verify_number(previous_reg)) {
    PRINTF("***ERROR*** in previous registration number\n");
    err = E_PREVIOUS_REG;
    goto error;
  }

no_prev_reg:

  if (ignore_prev_sess)
    goto no_prev_sess;

  PRINTF("Verifying previous session start\n");
  if ((strlen(previous_sess_start) < 4) || verify_number(previous_sess_start)) {
    PRINTF("***ERROR*** in previous session start\n");
    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry13");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

    err = E_PREVIOUS_SESS_START;
    goto error;
  }
  /* do some extra checking */
  if (atoi(previous_sess_start) >= atoi(all_session)) {
    PRINTF("***ERROR*** in previous session start\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry13");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

    err = E_PREVIOUS_SESS_START;
    goto error;
  }
  PRINTF("Verifying previous session end\n");
  if ((strlen(previous_sess_end) < 4) || verify_number(previous_sess_end)) {
    PRINTF("***ERROR*** in previous session end\n");
    err = E_PREVIOUS_SESS_END;
    goto error;
  }
  /* do some extra checking */
  if (atoi(previous_sess_end) >= atoi(all_session)) {
    PRINTF("***ERROR*** in previous session end\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry14");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

    err = E_PREVIOUS_SESS_END;
    goto error;
  }

  /* start must be one less than end */
  /* do some extra checking */
  if (atoi(previous_sess_start) != atoi(previous_sess_end)-1) {
    PRINTF("***ERROR*** in previous session start\n");

    /* grab focus to the widget */
    ptr = lookup_widget(GTK_WIDGET(data), "entry13");
    gtk_widget_grab_focus(GTK_WIDGET(ptr));

    err = E_PREVIOUS_SESS_START;
    goto error;
  }

no_prev_sess:
  /* now verify subject codes entered:
   * minimum 10 subjects must be entered
   * 16th subject is the fourth subject
   * duplicate entries must be handled
   */
  /* verify_subject_codes() will take
   * a widget pointer so that subject code entries
   * can be focussed. Firstly, it will check the subject
   * code hash table if the subject code is present in the
   * hash table. If present, it will check its own singly linked list
   * if this subject is already checked. If the subject is already present,
   * trigger error, otherwise append it to the list.
   * It will also call show_message() to inform the entry operator
   * and also highlight the last found duplicate entry
   * after a duplicate entry, it won't go on checking on
   * the rest of the subject codes.
   * some entries may be kept empty,
   * but duplicate entries are highly restricted
   */
  /*
   * 'data' is needed to highlight the widget
   * entry15 - entry31 are subject code entry
   */

ssc:

  /* check group entry and select appropriate filename */
  i = 0;
  while (groupfiles[i][0]) { /* iterate until NULL */
    if (!strcmp(group, groupfiles[i][0]))
      break;
    i++;
  }

  if (verify_subject_codes_by_group(data,
	(const char **) subs, groupfiles[i][1], groupfiles[i][2])) {
    PRINTF("***ERROR*** in subject code\n");
    err = E_NO_ERROR_FAILED; /* because the routine will show the message */
    goto error; /* simply skip writing it to disk */
  }

  /*
  for (i=0; i<NUM_TOTAL_SUB; i++) {
    PRINTF("Checking subject code %d\n", i);
    if ((strlen(subs[i]) < 3) || verify_sub_code(subs[i], (const char **) subs)) {
      PRINTF("***ERROR*** in subject code %d\n", i);
      err = E_SUB_CODE;
    }
  }
  */


  PRINTF("\n**verification complete**\n");
  err = E_NO_ERROR;

  /* now construct a path for the student */
  char full_filename[MAXDIRNAME+MAXFILENAME];
  char filename[MAXFILENAME];

#ifdef ____NO____
  /* add starting point */
  sprintf(infodirname, "%s/", ROOTDIR);
  PRINTF("infodirname = %s\n", infodirname);
  if ((res = MKDIR(infodirname, 0755)) != 0) {
    PRINTF("mkdir: %s: %s\n", infodirname, strerror(errno));
  }
  /* dirname => /home/ayub/ */

  /* append board */
  strcat(infodirname, board);
  PRINTF("infodirname = %s\n", infodirname);
  if ((res = MKDIR(infodirname, 0755)) != 0) {
    PRINTF("mkdir: %s: %s\n", infodirname, strerror(errno));
  }
  strcat(infodirname, "/");
  /* dirname => /home/ayub/Rajshahi/ */

  /* append session */
  strcat(infodirname, session);
  PRINTF("infodirname = %s\n", infodirname);
  if ((res = MKDIR(infodirname, 0755)) != 0) {
    PRINTF("mkdir: %s: %s\n", infodirname, strerror(errno));
  }
  strcat(infodirname, "/");
  /* dirname => /home/ayub/Rajshahi/2005/ */

  /* append college */
  strcat(infodirname, "INSTITUTE_");
  strcat(infodirname, college_code);
  PRINTF("infodirname = %s\n", infodirname);
  if ((res = MKDIR(infodirname, 0755)) != 0) {
    PRINTF("mkdir: %s: %s\n", infodirname, strerror(errno));
  }
  strcat(infodirname, "/");
  /* dirname => /home/ayub/Rajshahi/2005/INSTITUTE_XXXX/ */

  /* infodirname constructed and ready to use */
#endif /* ____NO____ */

  strcpy(full_filename, infodirname);
  /* full_filename => /home/ayub/Rajshahi/2005/INSTITUTE_3006/" */

  /* construct filename */
  classroll = (int) strtol(class_roll, (char **) NULL, 10); /* convert it to integer */
  int totlist = (int) strtol(tot_list, (char **) NULL, 10); /* convert to integer */

  construct_filename(filename, gender, classroll, section, totlist, group);
//  printf("Constructed filename : %s\n", filename);

  strcat(full_filename, filename);
  PRINTF("full_filename = %s\n", full_filename);

  /* now full_filename contains the name of the file path */
  fp = fopen(full_filename, "w");
  if (fp == NULL) {
    PRINTF("%s: ", full_filename);
    PRINTF("fopen: %s: %s\n", full_filename, strerror(errno));
    err = E_FILE_OPEN;
    goto error;
  }

  int sent = 0;   /* already sent? */

  fprintf(fp, HEADPATTERN, gender, classroll, section,
      totlist, group, sent, student_name);

  fprintf(fp, "STUDENTS_NAME %s\n", student_name);
  fprintf(fp, "FATHERS_NAME %s\n", father_name);
  fprintf(fp, "MOTHERS_NAME %s\n", mother_name);
  fprintf(fp, "GROUP %s\n", group);
  fprintf(fp, "SESSION %s\n", session);
  fprintf(fp, "BOARD %s\n", board);
  fprintf(fp, "CLASS_ROLL %04d\n", atoi(class_roll));
  fprintf(fp, "BIRTHDATE %s\n", birthdate);
  fprintf(fp, "TOT_LIST %04d\n", atoi(tot_list));
  fprintf(fp, "SECTION %s\n", section);
  fprintf(fp, "INSTITUTE_CODE %s\n", college_code);
  fprintf(fp, "INSTITUTE_NAME %s\n", college_name);
  fprintf(fp, "PREVIOUS_BOARD %s\n", previous_board);
  fprintf(fp, "PREVIOUS_YEAR %s\n", previous_year);
  fprintf(fp, "PREVIOUS_ROLL %s\n", previous_roll);
  fprintf(fp, "PREVIOUS_REG %s\n", previous_reg);
  fprintf(fp, "PREVIOUS_SESSION %s-%s\n", previous_sess_start,
      previous_sess_end);
  fprintf(fp, "GENDER %s\n", gender);
  fprintf(fp, "RELIGION %s\n", religion);

  /* in file, subject sequence starts from 0 to 14 */
  for (i=0; i<NUM_TOTAL_SUB; i++) {
    fprintf(fp, "SUB_%02d %s\n", i+1, subs[i]); /* SUB_XX */
  }

  /* now check photo location if supplied */
  char *photo_filename = NULL; /* full filename (used to copy) */
  char *photo_base = NULL; /* base filename */
  char *photo_ext = NULL; /* extension */
  ptr = lookup_widget(GTK_WIDGET(data), "photo_frame");
  if (ptr) {
    photo_filename = g_object_get_data(G_OBJECT(ptr), "photo");
    if (photo_filename) {
      PRINTF("Photo PATH = %s\n", photo_filename);
      photo_base = strrchr(photo_filename, '/');
      photo_base += 1; /* skip the '/' */
      PRINTF("Photo = %s\n", photo_base);
      photo_ext = strrchr(photo_base, '.');
      photo_ext += 1; /* skip '.' */
    }
  }

  /* photo finished */
  if (photo_base) {
    strcpy(&full_filename[strlen(full_filename)-3], photo_ext);
  }

  fprintf(fp, "PHOTO %s\n", photo_base ? full_filename : "");
  copy_photo(photo_filename, full_filename);

  /* clear photo */
  GList *children = gtk_container_get_children(GTK_CONTAINER(ptr));
  while(children) {
    if (GTK_WIDGET_TYPE(children->data) == GTK_TYPE_IMAGE) {
      gtk_container_remove(GTK_CONTAINER(ptr), children->data); /* remove previous pic */
      /* after gtk_container_remove(), the image is destroyed
       * automatically
       */
//	  gtk_widget_destroy(GTK_WIDGET(children->data));
    }
    children = children->next; /* iterate */
  }

  GtkWidget *photo_hint = create_pixmap(ptr, "photo-hint.png");
  if (photo_hint) {
    gtk_widget_show(photo_hint);
    gtk_container_add(GTK_CONTAINER(ptr), photo_hint);
  }

  PRINTF("Closing file\n");
  /* close the file */
  fclose(fp);

  char *msg = "Information Saved Successfully";
  show_message(msg);

  err = E_NO_ERROR; /* everything seems to be fine */

  /* now
   *   Student's Name:
   *   Father's Name:
   *   Mother's Name:
   * etc. entries should be cleared
   */

  if (!clear)
    goto noclear;

  /* find student's name entry widget */
  ptr = lookup_widget(GTK_WIDGET(data), "entry2");
  /* clear it */
  if (ptr)
    gtk_entry_set_text(GTK_ENTRY(ptr), "");
  /* find father's name entry widget */
  ptr = lookup_widget(GTK_WIDGET(data), "entry3");
  /* clear it */
  if (ptr)
    gtk_entry_set_text(GTK_ENTRY(ptr), "");
  /* find mother's name entry widget */
  ptr = lookup_widget(GTK_WIDGET(data), "entry4");
  /* clear it */
  if (ptr)
    gtk_entry_set_text(GTK_ENTRY(ptr), "");
  /* find class roll entry widget */
  ptr = lookup_widget(GTK_WIDGET(data), "entry6");
  /* clear it */
  if (ptr)
    gtk_entry_set_text(GTK_ENTRY(ptr), "");
  /* find S. S. C roll entry widget */
  ptr = lookup_widget(GTK_WIDGET(data), "entry11");
  /* clear it */
  if (ptr)
    gtk_entry_set_text(GTK_ENTRY(ptr), "");
  /* find S. S. C registration entry widget */
  ptr = lookup_widget(GTK_WIDGET(data), "entry12");
  /* clear it */
  if (ptr)
    gtk_entry_set_text(GTK_ENTRY(ptr), "");

noclear:

  /* grab focus to the student's name */
  ptr = lookup_widget(GTK_WIDGET(data), "entry2");
  gtk_widget_grab_focus(GTK_WIDGET(ptr));

error:
  PRINTF("Freeing memory\n");

  free(student_name);
  free(father_name);
  free(mother_name);
  free(group);
  free(session);
  free(board);
  free(class_roll);
  free(day_of_birth);
  free(month_of_birth);
  free(year_of_birth);
  free(tot_list);
  free(section);
  free(college_code);
  free(college_name);
  free(previous_board);
  free(previous_year);
  free(previous_roll);
  free(previous_reg);
  free(previous_sess_start);
  free(previous_sess_end);
  free(gender);
  free(religion);

  for (i=0; i<NUM_TOTAL_SUB; i++)
    free(subs[i]);




  PRINTF("**Complete**\n");
  return err;
}

int save_form(gpointer data)
{
  return 0;
}

#ifdef _WIN32

/* loads winsock.dll
 * returns 0 on success
 */
void init_winsock(void)
{
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;
  wVersionRequested = MAKEWORD(1, 1);

  err = WSAStartup(wVersionRequested, &wsaData);
  if (err != 0) {
    PRINTF("init_winsock: Usable winsock.dll can't be found!\n");
    return;
  }
  /* confirm that the Windows Socket DLL supports 1.1
   * note that if the DLL supports versions greater than
   * 1.1, in addition to 1.1, it will still return 1.1
   * in wVersion since it is the version we requested
   */
  if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
    PRINTF("init_winsock: Requested version not supported!\n");
    return;
  }

  winsock_ok = 1; /* now you can use socket functions */
}

#endif

/* 'host' may be hostname or ip address */
int init_connection(const char *host, int port)
{
#ifdef _WIN32
  SOCKET sock;
#else
  int sock;
#endif
  struct sockaddr_in servaddr; /* server socket address structure */
  int i;
//  struct timeval tv;
  struct hostent *hostinfo;
  char **pptr;
  struct in_addr in;

  int errval = 0;

  static char init_error_msg[512]; /* information message related to this function only */

//  tv.tv_sec = TIMEOUT;
//  tv.tv_usec = 0;

#ifdef _WIN32
  if (winsock_ok == 0)
    init_winsock();

  if (winsock_ok == 0) { /* still winsock.dll failed */
    show_message("winsock.dll can't be initialized");
    return -1;
  }

#endif


  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = PF_INET;        /* socket address family to use */
  servaddr.sin_port = htons(port); /* port of the server */

  while(gtk_events_pending())
    gtk_main_iteration();

  PRINTF("host = %s\n", host);
//  strcpy(webaddr, host);
  if (!lookup_ok) { /* not already looked up */
    PRINTF("looking up host : %s\n", host);
    hostinfo = gethostbyname(host);
    if (!hostinfo) {
      sprintf(init_error_msg, "<span foreground=\"red\">Hostname '<b>%s</b>' lookup failure"
       "\n      h_errno = %d", host, h_errno);
      switch(h_errno) {
	case HOST_NOT_FOUND:
	  strcat(init_error_msg, " (Unknown host)");
	  break;
	case NO_ADDRESS:
//	case NO_DATA:
	  strcat(init_error_msg, " (Host does not have an IP address)");
	  break;
	case NO_RECOVERY:
	  strcat(init_error_msg, " (Non-recoverable name server error occurred)");
	  break;
	case TRY_AGAIN:
	  strcat(init_error_msg, " (Temporary error occurred)");
	  break;
	default:
	  strcat(init_error_msg, " (Unknown error)");
	  break;
      }
      strcat(init_error_msg, "</span>");

      errval = -1;
      if (strlen(webaddr) > 8) { /* if address supplied in website.txt, use it */
	errval = 2;
	strcat(init_error_msg, "\n    <span foreground=\"maroon\">Trying ");
	strcat(init_error_msg, webaddr);
	strcat(init_error_msg, "</span>");
	/* winsock.h doesn't have inet_aton()
	 * use inet_addr() in this case
	 */
#ifdef _WIN32
	if ((servaddr.sin_addr.s_addr = inet_addr(webaddr)) == INADDR_NONE) {
#else
	if (inet_aton(webaddr, &servaddr.sin_addr) == 0) {
#endif
	  PRINTF("invalid address supplied\n");
	  strcat(init_error_msg, "\nInvalid address ");
	  strcat(init_error_msg, webaddr);
	  show_message(init_error_msg);
	  return -1;
	} /* end if */
      }
      show_message(init_error_msg);
      if (errval == -1)
	return errval; /* error! can't go further */
    }
    else {
      lookup_ok = 1; /* lookup successful, don't lookup again */
      pptr = hostinfo->h_addr_list;
      memcpy(&in, *pptr, sizeof(struct in_addr));
      strcpy(webaddr, inet_ntoa(in));
      PRINTF("ip = %s\n", webaddr);
      memcpy(&servaddr.sin_addr, &in, sizeof(in));
      PRINTF("s_addr = %d\n", servaddr.sin_addr.s_addr);
    }
  } /* if (!lookup_ok) */
  else {
    /* use previous successful address */
#ifdef _WIN32
    servaddr.sin_addr.s_addr = inet_addr(webaddr);
#else
    inet_aton(webaddr, &servaddr.sin_addr);
#endif
//    PRINTF("s_addr = %d\n", servaddr.sin_addr.s_addr);
    strcpy(init_error_msg, "");
//    memset(init_error_msg, 0, sizeof(init_error_msg));
  }

//  char ipaddr[16];             /* server ip address */
//  char buffer[MAXLINE];
//  int ok = 0;

  for (i = 1; i <= 30; i++) {
    switch(i) {
      case SIGINT:
      case SIGTERM:
      case SIGSEGV:
	signal(i, handle_signal);
	break;
      default:
	signal(i, SIG_IGN); /* ignore signal */
    }
  }

  /* a valid address supplied,
   * so creating socket for this connection is meaningful */
#ifdef _WIN32
  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
#else
  if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
#endif
    show_message("Socket failed");
//    PRINTF("socket: %s\n", strerror (errno));
    return -1;
  }

  /* set the timeout */
  /*
   * commenting this out because this is buggy in Windows(R)
   * also, not all versions of windows suppor this
   *
#ifdef _WIN32
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char FAR *) &tv, sizeof(tv));
  setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char FAR *) &tv, sizeof(tv));
#else
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (void *) &tv, sizeof(tv));
  setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (void *) &tv, sizeof(tv));
#endif
*/


  while(gtk_events_pending())
    gtk_main_iteration();

  PRINTF("Connecting to server : %-30s", webaddr);
  if (connect(sock, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1) {
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    PRINTF("[FAILED]\n");
    strcat(init_error_msg, "\n  Connect Failed!!! Check the following:"
	"\n    1. Make sure you are connected to the Internet"
	"\n    2. You are not behind a proxy or firewall"
	"\n    3. Make sure that the server is running"
	"\n  Contact your system administrator if this problem persists"
	);
    show_message(init_error_msg);
    return (-1);
  }
  PRINTF("[  OK  ]\n");

  return sock;
}

void *apply_forms(void *val)
{
  FILE *fp;
  DIR *info = NULL;
  struct dirent *file;
  char full_filename[MAXDIRNAME+MAXFILENAME];
  int len;
  char sec[2];
  char gen[7];
  int roll;
  int sent;
  char buffer[MAXLINE]; /* only first line will be read */

  GtkWidget *ptr;
  int success = 0;
  int failure = 0;

//  int val;
  int ret = 0;
  int nwrite;

  GString *msg = NULL;

  int tot;
  char group[20];

  /*
  if (!formdir)
    formdir = strdup(dir);
  if (!send_session)
    send_session = strdup(session);
    */


//  sprintf(login, "%04d", all_col_code);

  if (strlen(pass) == 0) {
    ask_pass();
    return NULL;
  }


  submit_cancelled = 0;

  if (!online_transfer_win)
    online_transfer_win = online_transfer_window ();

  if (!GTK_WIDGET_VISIBLE(online_transfer_win))
    gtk_widget_show(online_transfer_win);

//  sprintf(error_msg, "Connecting to <span foreground=\"maroon\"><b>%s</b> (%s)</span>     ", website, webaddr);
//  sleep(3);
//  PRINTF("Initializing connection\n");
//  sprintf(error_msg, "Connecting to %s", website);
//  sockfd = init_connection(webaddr, SERV_PORT);
  ptr = lookup_widget(online_transfer_win, "label2");
  if (ptr)
    gtk_label_set_text(GTK_LABEL(ptr), ""); /* clear previous transfer status */

  sprintf(error_msg, "Connection Status:\nConnecting to <span foreground=\"maroon\"><b>%s</b></span>", website);
  msg = g_string_new(error_msg);
  ptr = lookup_widget(online_transfer_win, "label1");
  if (ptr)
    gtk_label_set_markup(GTK_LABEL(ptr), msg->str);


  sockfd = init_connection(website, SERV_PORT);

  g_string_append(msg, " (<span foreground=\"#205326\"><b>"); /* name should be looked up by init_connection() */
  g_string_append(msg, (strlen(webaddr) > 8) ? webaddr : "unresolved");
  g_string_append(msg, "</b></span>)      ");
  gtk_label_set_markup(GTK_LABEL(ptr), msg->str);
  while(gtk_events_pending())
    gtk_main_iteration();

  if (sockfd == -1) {
    g_string_append(msg, "<span foreground=\"red\"><b>[FAILED]</b></span>");
    gtk_label_set_markup(GTK_LABEL(ptr), msg->str);
    while(gtk_events_pending()) /* so that user can see progress */
      gtk_main_iteration();
    g_string_free(msg, TRUE);
//    sleep(3);
    return NULL;
  }
  else if (authenticate(all_colcode, pass)) {
//    PRINTF("Authentication failure\n");
//    show_message("Authentication Failure");
    memset(pass, 0, sizeof(pass)); /* new password needed */
#ifdef _WIN32
    closesocket(sockfd);
#else
    close(sockfd);
#endif
    sockfd = 0;
    g_string_free(msg, TRUE);
    return NULL;
  }

  /* if authentication succeeds, we can use this host address.
   * since each lookup costs much time
   * */
  lookup_ok = 1;

  g_string_free(msg, TRUE);

  /*
  val = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, val | O_NONBLOCK);
  */

//  SLEEP(3);

#ifdef ____NO____
  sprintf(buffer, "SESSION %s\n", all_session);

  nwrite = strlen(buffer);
  if ((ret = writen(sockfd, buffer, nwrite)) != nwrite) {
#ifdef _WIN32
    show_message("Error sending session");
    closesocket(sockfd);
#else
    sprintf(buffer, "ERROR : SESSION : send : %s", strerror(errno));
    show_message(buffer);
    close(sockfd);
#endif
    return NULL;
  }

#endif

//  PRINTF("Apply forms %s*.txt\n", dir);

//  PRINTF("Opening directory : %s\n", dir);

  if (submit_cancelled)
    goto write_quit;

  info = opendir(infodirname);
  if (!info) {
    PRINTF("opendir : %s : %s\n", infodirname, strerror(errno));
    if (GTK_WIDGET_VISIBLE(online_transfer_win))
      gtk_widget_hide(online_transfer_win);
    goto write_quit;
  }
  /* directory opened */
  while ((file = readdir(info))) {
    if (file->d_name[0] == '.')
      continue;
    len = strlen(file->d_name);
    if (len < MINFILENAMELEN || len > MAXFILENAMELEN)
      continue; /* not our info file */
    if (strcmp(&file->d_name[len-4], ".txt"))
      continue; /* skip non-text files */
    strcpy(full_filename, infodirname);
    if (infodirname[strlen(infodirname)-1] != '/') /* add '/' if not present */
      strcat(full_filename, "/");
    strcat(full_filename, file->d_name);

    PRINTF("Opening %s\n", full_filename);
    fp = fopen(full_filename, "r"); /* just for checking if it's sent */
    if (!fp) {
      PRINTF("fopen : %s : %s\n", full_filename, strerror(errno));
      continue;
    }
    fgets(buffer, sizeof(buffer), fp);

    sscanf(buffer, "%[^:]:%d:%[^:]:%d:%[^:]:%d:", gen, &roll, sec, &tot, group, &sent);
    fclose(fp);
    if (!sent) { /* not sent */
      sprintf(buffer, "FILENAME %s\n", file->d_name);
      PRINTF(buffer, "FILENAME |%s|\n", file->d_name);
      nwrite = strlen(buffer);
      if ((ret = writen(sockfd, buffer, nwrite)) != nwrite) {
#ifdef _WIN32
	show_message("Error sending data");
	closesocket(sockfd);
#else
	sprintf(buffer, "ERROR : FILENAME : send : %s", strerror(errno));
	show_message(strerror(errno));
	close(sockfd);
#endif
	sockfd = 0;
	show_message(buffer);
	ret = 1;
	goto free_mem;
      }
      if (submit_cancelled)
	goto write_quit;
      sprintf(error_msg, "Transfer Status: %d success, %d failed.\n"
	  "Sending <span foreground=\"maroon\"><b>%s</b></span>, Please wait...", success, failure, file->d_name);
      ptr = lookup_widget(online_transfer_win, "label2");
      if (ptr)
	gtk_label_set_markup(GTK_LABEL(ptr), error_msg);

      while(gtk_events_pending()) /* so that user can see progress */
	gtk_main_iteration();
      /* just to see the progress */
//      SLEEP(3);
      /* ******************** */
      if (apply_form(full_filename) == 0)
	success++;
      else
	failure++;
    }
  }

write_quit:
  if (sockfd > 0)
    writen(sockfd, "QUIT\n", 5);

free_mem:
  /*
  if (!GTK_WIDGET_VISIBLE(online_transfer_win))
    gtk_widget_show(online_transfer_win);
    */

  sprintf(error_msg, "Transfer Status: <b>%d</b> success, <b>%d</b> failed.\n", success, failure);
  ptr = lookup_widget(online_transfer_win, "label2");
  if (ptr)
    gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
  ptr = lookup_widget(online_transfer_win, "button1");
  if (ptr && GTK_WIDGET_VISIBLE(ptr))
    gtk_widget_hide(ptr);
  ptr = lookup_widget(online_transfer_win, "button2");
  if (ptr && !GTK_WIDGET_VISIBLE(ptr)) {
    gtk_widget_show(ptr);
    gtk_widget_grab_focus(ptr);
  }

  closedir(info);
  if (sockfd > 0) {
//    PRINTF("Closing socket\n");
//    PRINTF("Shutting down socket write end\n");
//    close(sockfd);
#ifdef _WIN32
    shutdown(sockfd, SD_SEND);
    closesocket(sockfd);
#else
    shutdown(sockfd, SHUT_WR);
    close(sockfd);
#endif
    sockfd = 0;
  }

  return NULL;
}

/* apply individual form */
/* including pics (if enabled) */
int apply_form(const char *filename)
{
  FILE *fp = NULL;
//  char base[20]; /* name without extension */
  static char buffer[MAXLINE]; /* read buffer */

  static char photo[MAXDIRNAME+MAXFILENAME];

  /* header info */
  char gen[7]; /* gender */
  int roll;
  char sec[2]; /* section */
  int totlist;
  static char group[20]; /* group */
  int sent;
  static char st_name[41]; /* student's name */
  /* ********** */

  static char rbuf[20]; /* socket read buffer */

  int ret;
  int err = 0;
  int nwrite;

//  PRINTF("Apply form : %s\n", filename);
  
//  freopen (LOGFILE, "w", stdout);
  

  memset(rbuf, 0, sizeof(rbuf));
  memset(buffer, 0, sizeof(buffer));

  fp = fopen(filename, "r"); /* open for reading */
  if (!fp) {
    PRINTF("%s : ", filename);
    PRINTF("fopen: %s: %s\n", filename, strerror(errno));
    show_message(strerror(errno));
    err = 2;
    return 1;
  }

  memset(photo, 0, sizeof(photo));

  while (fgets(buffer, sizeof(buffer), fp)) {
    if (submit_cancelled) {
      err = 1;
      goto finalize;
    }

    if (!strncmp(buffer, "PHOTO ", 6)) /* retrieve photo name */
      sscanf(buffer, "PHOTO %[^\r\n]", photo);

//    PRINTF("FGETS: %s", buffer);
    nwrite = strlen(buffer);
    if (writen(sockfd, buffer, nwrite) != nwrite) {
#ifdef _WIN32
      show_message("Error sending data");
#else
      PRINTF("error: send: %s\n", strerror(errno));
      show_message(strerror(errno));
#endif
      fclose(fp);
      return 3;
    }
  }
  if (writen(sockfd, ".\n", 2) < 2) {
#ifdef _WIN32
    show_message("Error sending data");
#else
    PRINTF("ERROR : send : %s\n", strerror(errno));
    show_message(strerror(errno));
#endif
    fclose(fp);
    return 1;
  }

  /* readline() sets extra terminating null byte to buffer */
  PRINTF("Waiting for feedback from server\n");
  if ((ret = readline(sockfd, rbuf, sizeof(rbuf))) != 4) {
    PRINTF("[FAILED]\n");
//    perror("readline");
    return 1;
  }
  if (!strncmp(rbuf, "500\n", 4)) {
    PRINTF("SERVER %s", rbuf);
    err = 0;
  }
  else {
    PRINTF("SERVER %s", rbuf);
    err = 1;
  }

//  sleep(1);
//  err = 0; /* for now i'm assuming it */

finalize:
  if (fp)
    fclose(fp);

  if (err == 0) {
    /* sent successfully, feedback got */
    /* update the sent flag to the file */
    fp = fopen(filename, "r+");
    if (!fp) {
      err = 1;
    }
    else {
      fgets(buffer, sizeof(buffer), fp); /* read first line (including newline ) */
      sscanf(buffer, "%[^:]:%d:%[^:]:%d:%[^:]:%d:%[^\r\n]",
	  gen, &roll, sec, &totlist, group, &sent, st_name);
      rewind(fp); /* again reset */
      fprintf(fp, "%s:%04d:%s:%04d:%s:%d:%s\n",
	  gen, roll, sec, totlist, group, 1, st_name); /* sent */

      /* everything is done successfully */
      fclose(fp);
      PRINTF("SUCCESS\n");

      refresh_editor_list();
      err = 0;
    }
  }

  if (ignore_photo)
    return err; /* no need to send photo */

  if (strlen(photo) < FILENAMELEN) /* no photo name in registration file */
    return err;

  send_photo(photo);

  return err;

  /*
  else
    return 1;
   */
}


/* more than one space is prohibited */
/* lowercase strings are allowed because
 * that string may be made uppercase by
 * strupper()
 */
int verify_name(const char *str)
{
  int num_space = 0; /* number of consecutive spaces */

//  if (!(isalpha(*str) && isupper(*str)))
  if (!(isalpha(*str))) {
    PRINTF("Alphabet required\n");
    return 1;
  }
  str++;
  while (*str != '\0') {
//    if ((isalpha(*str) && isupper(*str)) || (*str == '.' || *str == '-')) {
    if (isalpha(*str) || (*str == '.' || *str == '-')) {
      str++;
      num_space = 0;
    }
    else if (isspace(*str) && (num_space < 1)) {
      str++;
      num_space++;
    }
    else
      return 1;
  }

  return 0;
}

/* verify_subject_codes_by_group */
int verify_subject_codes_by_group(GtkWidget *widget,
    const char **codes, const char *filename, const char *fourthsubfile)
{
  GSList *local_list = NULL; /* from entries */
  GSList *sublist = NULL; /* from 'filename' */
  GSList *fourthsublist = NULL; /* from 'fourthsubfile' */
  GSList *special = NULL; /* special codes */
  GtkWidget *ptr;
  int err = 0;
  int ret;

  FILE *fp;

  int subidx; /* index of subject codes */
  char entryname[10];

  int len; /* length of entry string */
  int codeval;

  int total_sub; /* total subject codes to check for */
  int fourth;
  int i;

  fp = fopen(filename, "r");
  if (!fp) {
    PRINTF("fopen: %s: %s\n", filename, strerror(errno));
    err = 1;
    return err;
  }

  PRINTF("Creating list from %s\n", filename);
  while(fscanf(fp, "%d", &codeval) > 0) { /* while input available */
    sublist = g_slist_append(sublist, GINT_TO_POINTER(codeval));
//    PRINTF("%s : list DUMP : \n", filename);
//    print_slist_int(sublist); /* just see if i'm correct */
  }
  
  /* subject code list created from filename */
  fclose(fp);

  fp = fopen(fourthsubfile, "r");
  if (!fp) {
    PRINTF("fopen: %s: %s\n", fourthsubfile, strerror(errno));
    err = 1;
    return err;
  }

  PRINTF("Creating fourth subject list from %s\n", fourthsubfile);
  while(fscanf(fp, "%d", &codeval) > 0) { /* while input available */
    fourthsublist = g_slist_append(fourthsublist, GINT_TO_POINTER(codeval));
//    PRINTF("%s : list DUMP : \n", filename);
//    print_slist_int(sublist); /* just see if i'm correct */
  }
  
  /* subject code list created from filename */
  fclose(fp);

  /* check for fourth subject and decide loop length */
  fourth = (int) strtol(codes[NUM_TOTAL_SUB-1], (char **) NULL, 10); /* fourth subject first paper */
  if (fourth == 0) {
    PRINTF("fourth = 0\n");
//    with_f = 0; /* without fourth subject */
    total_sub = WITHOUT_F_MIN;
  }
  else if (fourth < 100) { /* invalid code */
    sprintf(entryname, "entry%d", NUM_TOTAL_SUB-1+15);
    ptr = lookup_widget(GTK_WIDGET(widget), entryname);
    if (ptr) /* found, now hilight it */
      gtk_widget_grab_focus(ptr);
    strcpy(error_msg, "Invalid subject code entry");
    show_message(error_msg);
    err = 1;
    goto finish;
  }
  else {
    if (!g_slist_find(fourthsublist, GINT_TO_POINTER(fourth))) {
      sprintf(entryname, "entry%d", NUM_TOTAL_SUB-1+15);
      ptr = lookup_widget(GTK_WIDGET(widget), entryname);
      if (ptr) /* found, now hilight it */
	gtk_widget_grab_focus(ptr);
      strcpy(error_msg, "Invalid fourth subject code!"
	  "   \nCheck your code syllabus for valid code");
      show_message(error_msg);
      err = 1;
      goto finish;
    }
//    with_f = 1; /* with fourth subject */
    total_sub = WITH_F_MIN;
  }

  fp = fopen(SPECIALFILE, "r");
  if (!fp) {
    PRINTF("fopen: %s: %s\n", SPECIALFILE, strerror(errno));
    err = 1;
    return err;
  }

  PRINTF("Creating special list from %s\n", SPECIALFILE);
  while(fscanf(fp, "%d", &codeval) > 0) { /* while input available */
    special = g_slist_append(special, GINT_TO_POINTER(codeval));
//    PRINTF("%s : list DUMP : \n", filename);
//    print_slist_int(sublist); /* just see if i'm correct */
  }
  
  /* subject code list created from filename */
  fclose(fp);
//  PRINTF("created\n");

  /* now check if special code is filled in.
   * it will increase the total_sub by one
   * only one special code may be filled in for a single group
   * */
  i = 0;
  while (codes[i]) {
    codeval = (int) strtol(codes[i], (char **) NULL, 10);
    if (g_slist_find(special, GINT_TO_POINTER(codeval))) {
      total_sub += 1;
//      break;
    }
    i++;
  }

  /* special subject code is now not bound.
   * any number of special subject codes are allowed
   */

  for (subidx=0; subidx < total_sub; subidx++) {
    len = strlen(codes[subidx]);
    if (!len) { /* entry not filled up */
      sprintf(entryname, "entry%d", subidx+15);
      ptr = lookup_widget(GTK_WIDGET(widget), entryname);
      if (ptr) /* found, now hilight it */
	gtk_widget_grab_focus(ptr);
      sprintf(error_msg, "Entry empty!!!"
	  "\nFirst %d entries must be filled in", total_sub);
      show_message(error_msg);
      err = 1;
      goto finish;
    }
    if (len < 3) { /* invalid entry */
      sprintf(entryname, "entry%d", subidx+15);
      ptr = lookup_widget(GTK_WIDGET(widget), entryname);
      if (ptr) /* found, now hilight it */
	gtk_widget_grab_focus(ptr);
      strcpy(error_msg, "Invalid subject code entry");
      show_message(error_msg);
      err = 1;
      goto finish;
    }

    /* now check if it is present in sublist */
    codeval = (int) strtol(codes[subidx], (char **) NULL, 10);
    if (!g_slist_find(sublist, GINT_TO_POINTER(codeval))) { /* if NULL (not found) */
      /* lookup entry widget and highlight it */
      sprintf(entryname, "entry%d", subidx+15);
      PRINTF("%s->lookup\n", entryname);
      ptr = lookup_widget(GTK_WIDGET(widget), entryname);
      if (ptr) { /* found, now hilight it */
	gtk_widget_grab_focus(ptr);
      }
      strcpy(error_msg, "Subject code entry not present in list");
      show_message(error_msg); /* display it */
      err = 1;
      goto finish;
    }
    /* subject code valid for the group */

    /* now check for duplicate entries */
    if(!g_slist_find(local_list, GINT_TO_POINTER(codeval))) { /* not found */
      local_list = g_slist_append(local_list,
	  GINT_TO_POINTER(codeval)); /* append it */
//      PRINTF("Filled subject code list DUMP : \n");
//      print_slist_int(local_list); /* just see if i'm correct */
    }
    else { /* duplicate entry found */
      /* lookup entry widget and highlight it */
      sprintf(entryname, "entry%d", subidx+15);
      PRINTF("%s->lookup\n", entryname);
      ptr = lookup_widget(GTK_WIDGET(widget), entryname);
      if (ptr) { /* found, now highlight it */
	gtk_widget_grab_focus(ptr);
      }
      sprintf(error_msg, "Duplicate subject code entry : %d", codeval);
      show_message(error_msg); /* display it */
      err = 1;
      goto finish;
    }
  }

  /* local_list now contains list of filled codes */


  /* now pair checking */
  if ((ret = paircheck(local_list))) { /* check pair matching for filled codes */
    if (ret < 0) {
      strcpy(error_msg, "Unknown error while pair matching!");
      show_message(error_msg);
    }
    else {
      sprintf(entryname, "entry%d", ret+15-1); /* paircheck will return incremented value */
      PRINTF("%s->lookup\n", entryname);
      ptr = lookup_widget(GTK_WIDGET(widget), entryname);
      if (ptr) /* found, now highlight it */
	gtk_widget_grab_focus(ptr);
      strcpy(error_msg, "Subject code pair mismatch!!!\nCheck your subject code syllabus");
      show_message(error_msg);
    }
    err = 1;
    goto finish;
  }

  /* now check if any extra entry is filled in */
  if (total_sub >= 12) { /* fourth subject filled in, ckeck if it exists in filled list */
    if (!g_slist_find(local_list, GINT_TO_POINTER(fourth))) {
      show_message("Fourth subject must be present in filled list of codes!");
      err = 1;
      goto finish;
    }
  }
  /* now check if any extra entries are filled in */
  for ( ; subidx < NUM_TOTAL_SUB-1; subidx++) {
    if (strlen(codes[subidx]) > 0) {
      sprintf(entryname, "entry%d", subidx+15);
      PRINTF("%s->lookup\n", entryname);
      ptr = lookup_widget(GTK_WIDGET(widget), entryname);
      if (ptr) /* found, now highlight it */
	gtk_widget_grab_focus(ptr);
      sprintf(error_msg, "Extra entry supplied!"
	  "  \nCheck that you have filled the FOURTH SUBJECT FIRST PAPER\n"
	  "    entry in the fourth subject code entry box.");
      show_message(error_msg);
      err = 1;
      goto finish;
    }
  }

  err = 0; /* everything seems fine */
finish:
  g_slist_free(sublist);
  g_slist_free(special);
  g_slist_free(fourthsublist);
  g_slist_free(local_list); /* free resources */
  return err;
}


/* pair checking of codes */
int paircheck(GSList *filled_codes)
{
//  int err = 0;
  GSList *temp = NULL;
  GSList *valuep = NULL;
  int index; /* which index failed to match */
//  unsigned int code;
  int found = 1; /* because there may be lonepair code */
  int matched;
  int val;

  int tempval;

  create_pair_hash_table(PAIRFILE);
  if (!pairhtable)
    return -1; /* error */

  temp = filled_codes;
  if (!temp) /* NULL */
    return -1;

  index = 0;

  PRINTF("Checking pairs\n");
  PRINTF("Filled codes :\n");
//  print_slist_int(filled_codes);


  for ( ; temp; temp = g_slist_next(temp)) {
    found = 1; /* first assume that code has no pair */
    matched = -1;
    index += 1;
    if (!temp->data)
      continue;
    PRINTF("looking for hash key : ");
    val = GPOINTER_TO_INT(temp->data);
    PRINTF("%d", val);
    if ((valuep = g_hash_table_lookup(pairhtable, &val)) == NULL) {
      PRINTF(" NOT FOUND; lonepair code %d\n", val);
      continue;
    }
    PRINTF(" FOUND\n");
    /* pair list found */
    found = 0; /* pair match not yet successful */
    for ( ; !found && valuep; valuep = g_slist_next(valuep)) {
      tempval = GPOINTER_TO_INT(valuep->data);
      PRINTF("val = %d\n", tempval);
      switch(tempval) {
	case 2:
	    matched = 0; /* make it zero */
	    found = 0;
	    break;
	  break;
	case 1:
	  found = 0;
	  break;
	default:
	  PRINTF("default: finding %d in filled_codes ...", tempval);
	  if (g_slist_find(filled_codes, GINT_TO_POINTER(tempval))) {
	    PRINTF(" FOUND\n");
	    if (matched >= 0) {
	      matched++;
	      if (matched != 2)
		found = 0;
	      else
		found = 1;
	    }
	    else
	      found = 1;
	  }
	  else {
	    PRINTF(" NOT FOUND\n");
	    found = 0;
	  }
	  break;
      } /* end switch */
    } /* end inner for */
    if (!found)
      return index;
  }

  if (!found)
    return index;
  /* everything seems fine */
    return 0;
}

/* create pair hash table */
GHashTable *create_pair_hash_table(const char *filename)
{
//  GHashTable *htable = NULL;
  FILE *fp;
  int key;
  int *keyp;
  GSList *list = NULL;
  int value[10]; /* maximum 10 codes may be in a single line */
//  int *valuep;
  int items; /* number of input items in a line */
  int i; /* loop */

  char buffer[60];
//  int *codep; /* pointer to the code */
//  char *namep; /* pointer to the name */

  if (pairhtable) { /* if already created, return it */
    PRINTF("Subject code pair hash table already created; returning\n");
    return pairhtable;
  }

  if (!filename || !strlen(filename)) /* filename not supplied */
    return pairhtable; /* subhtable == NULL */

  fp = fopen(filename, "r");
  if (!fp) {
    PRINTF("fopen: %s: %s\n", filename, strerror(errno));
    return pairhtable; /* subhtable == NULL */
  }

  PRINTF("Creating subject code pair hash table\n");
  pairhtable = g_hash_table_new_full(g_int_hash,
      g_int_equal,
      free,  /* standard free() routine */
      (GDestroyNotify) g_slist_free); /* standard free list routine */


  PRINTF("Reading line:\n");
  while (fgets(buffer, sizeof(buffer), fp)) {
    PRINTF(buffer);
    memset(&key, 0, sizeof(key));
    memset(value, 0, sizeof(value));
    if ((items = sscanf(buffer, "%d%d%d%d%d%d%d%d%d%d", &key, &value[0], &value[1], &value[2], &value[3], &value[4], &value[5],
	    &value[6], &value[7], &value[8])) > 1) { /* while input available */
      PRINTF("items read = %d\n", items);
      list = NULL; /* new list */
      PRINTF("***new list***\n");
      keyp = (int *) malloc(sizeof(int));
      memcpy(keyp, &key, sizeof(key));
//      *keyp = key;
      PRINTF("key : %d ", *keyp);

      PRINTF("=> (");
      for (i=0; i<items-1; i++) { /* because first one is 'key' */
//	valuep = (int *) malloc(sizeof(int));
//	memcpy(valuep, &value[i], sizeof(int));
//	*valuep = value[i];
//	PRINTF(" %d ", *valuep);
	list = g_slist_append(list, GINT_TO_POINTER(value[i]));
      }
      PRINTF(")\n");

      /* now insert the list as the key value to hash table */
      g_hash_table_insert(pairhtable, keyp, list);
      PRINTF("Dump of list : \n");
//      print_slist_int(list);
    }
    else {
      PRINTF("Skipping line : %s", buffer);
    }
  }

  PRINTF("Pair hash table complete\n");
  fclose(fp);

  return pairhtable;
}



int verify_subject_codes(GtkWidget *widget, const char **codes)
{
  GSList *local_list = NULL;
  GtkWidget *ptr;

  int subidx; /* index of subject codes */
  char entryname[10];

  int len; /* length of entry string */
  int codeval;

  if (!subhtable) { /* subject hash table not already present */
    create_sub_hash_table(SUBCODEFILE); /* create hash table from file */
    if (!subhtable) /* error creating table */
      return 1;
  }

  for (subidx=0; subidx < NUM_TOTAL_SUB; subidx++) {
    len = strlen(codes[subidx]);
    if (!len) /* len == 0 */
      continue; /* entry not filled up, go to next entry */
    if (len < 3) /* invalid entry */
      return 1;

    /* now check if it is present in subhtable hash */
    codeval = (int) strtol(codes[subidx], (char **) NULL, 10);
    if (!g_hash_table_lookup(subhtable, &codeval)) { /* if NULL (not found) */
      /* lookup entry widget and highlight it */
      sprintf(entryname, "entry%d", subidx+15);
      PRINTF("%s->lookup\n", entryname);
      ptr = lookup_widget(GTK_WIDGET(widget), entryname);
      if (ptr) { /* found, now highlight it */
	gtk_widget_grab_focus(ptr);
      }
      strcpy(error_msg, "Subject code entry not present in table");
      show_message(error_msg); /* display it */
      return 1; /* error */
    }

    if(!g_slist_find(local_list, GINT_TO_POINTER(codeval))) { /* not found */
      local_list = g_slist_append(local_list,
	  GINT_TO_POINTER(codeval)); /* append it */
//      PRINTF("Filled subject code list DUMP : \n");
//      print_slist_int(local_list); /* just see if i'm correct */
    }
    else { /* duplicate entry found */
      /* lookup entry widget and highlight it */
      sprintf(entryname, "entry%d", subidx+15);
      PRINTF("%s->lookup\n", entryname);
      ptr = lookup_widget(GTK_WIDGET(widget), entryname);
      if (ptr) { /* found, now highlight it */
	gtk_widget_grab_focus(ptr);
      }
      sprintf(error_msg, "Duplicate subject code entry : %d", codeval);
      show_message(error_msg); /* display it */
      return 1; /* error */
    }
  }
  g_slist_free(local_list); /* free resources */
  return 0; /* subject codes are valid */

}

void print_slist_int(GSList *list)
{
  do {
    PRINTF("%d\n", GPOINTER_TO_INT(list->data));
  } while ((list = g_slist_next(list)));
}


/* check if subject code is within the range */
/* check if code is already present in codes */
int verify_sub_code(const char *code, const char **codes)
{
  int i;
//  int ch;

  if (!isdigit(*code) || (*code > '2')) {
    return 1;
  }

  for (i=1; i<3; i++) {
    if (!isdigit(*(code+i)))
      return 1;
  }

  return 0;
}

int verify_number(const char *str)
{
  PRINTF("**      verify number : %s**\n", str);
  if (strlen(str) < 1)
    return 1;
  while (*str != '\0') {
    if (!isdigit(*str++))
      return 1;
  }

  return 0;
}

int verify_session(const char *str)
{
  int i;

  if (*(str+2) != '-') /* xx - xx */
    return 1;

  for (i=0; i<6; i++) {
    if (i == 2) continue;
    if (!isdigit(*(str+1)))
      return 1;
  }

  return 0;
}

int verify_year(const char *str)
{
  return verify_number(str);
}

/* date is passed as 'dd-Mon-yyyy' */
int verify_date(int dd, char *Mon, int yyyy)
{
  int leap;


  leap = (yyyy%4 == 0 && yyyy%100 != 0) || (yyyy%400 == 0);

  if (!strcmp(Mon, "Feb")) { /* handle february month */
    if (leap && (dd > 29))
      return 1;
    else if (!leap && (dd > 28))
      return 1;
    else
      return 0; /* valid february month */
  }
  /* now you need to check for every 30 day months */
  else if (!strcmp(Mon, "Apr") && (dd > 30))
    return 1;
  else if (!strcmp(Mon, "Jun") && (dd > 30))
    return 1;
  else if (!strcmp(Mon, "Sep") && (dd > 30))
    return 1;
  else if (!strcmp(Mon, "Nov") && (dd > 30))
    return 1;
  else
    return 0;

  return 0;
}


/* row data */
enum {
  SUB_CODE, SUB_NAME, SUB_N_COLUMNS
};

const char *row_str[] = {
  "Subject Code", "Subject Name"
};


/* create and fill model with given data */
GtkTreeModel *create_model(void)
{
  GtkListStore *store;
  GtkTreeIter iter;
  char line[80]; /* maximum length of a line */
  int subcode; /* code of the subject */
  char subname[40]; /* name of the subject */

//  int state;

  FILE *fp = NULL;

  fp = fopen(SUBCODEFILE, "r");
  if (fp == NULL) {
    PRINTF("%s : ", SUBCODEFILE);
    PRINTF("fopen: %s: %s\n", SUBCODEFILE, strerror(errno));
    return NULL;
  }

  /* create list store */
  store = gtk_list_store_new (SUB_N_COLUMNS,
			      G_TYPE_UINT, /* Subject Code */
			      G_TYPE_STRING); /* Subject Name */
  while (fgets(line, sizeof(line), fp) != NULL) {
    gtk_list_store_append (store, &iter);
    /*
     * if you make any row G_TYPE_STRING, then sorting will occur lexically
     * if you make any row G_TYPE_{U,}INT, then sorting will occur
     * numerically
     */
    sscanf(line, "%d%[^\n]", &subcode, subname);

    gtk_list_store_set (store, &iter,
			SUB_CODE, subcode,
			SUB_NAME, subname,
			-1);
  }

  fclose(fp);
  PRINTF("Subject Code Index Created.\n");
  return GTK_TREE_MODEL (store);
}

void add_columns(GtkTreeView *treeview)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
//  GtkTreeModel *model = gtk_tree_view_get_model (treeview);

  /*
  renderer = gtk_cell_renderer_toggle_new ();
  g_signal_connect (renderer, "toggled",
		    G_CALLBACK (fixed_toggled), model);

  column = gtk_tree_view_column_new_with_attributes ("Fixed?",
						     renderer,
						     "active", FIXED,
						     NULL);

  gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN (column),
				   GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_fixed_width (GTK_TREE_VIEW_COLUMN (column), 50);
  gtk_tree_view_append_column (treeview, column);
  */

  /* column for information */
  int i;
  int len = ARRAY_LEN(row_str);


  for (i = 0; i < len; i++) {
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (row_str[i],
						       renderer,
						       "text",
						       i,
						       NULL);
    /* set the column sortable */
    gtk_tree_view_column_set_sort_column_id (column, i);
    gtk_tree_view_append_column (treeview, column);
  } /* for */
}

GtkWidget *do_list_store(GtkWidget *window)
{
  if (!window) {
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *sw;
    GtkTreeModel *model;
    GtkWidget *treeview;
    GtkWidget *indexbutton;

    /* create window, etc */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Subject Code Index");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
//  gtk_window_set_decorated(GTK_WINDOW(window), FALSE); /* no title bar */

    g_signal_connect (G_OBJECT(window), "delete_event",
		      G_CALLBACK (gtk_widget_hide), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 8);

    vbox = gtk_vbox_new (FALSE, 8);
    gtk_container_add (GTK_CONTAINER (window), vbox);

    label = gtk_label_new ("This is the subject code list");
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);

    sw = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
					 GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
				    GTK_POLICY_NEVER,
				    GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start (GTK_BOX (vbox), sw, TRUE, TRUE, 0);

    /* button to close the window (just hide it) */
    indexbutton = gtk_button_new_from_stock("gtk-close");
    gtk_box_pack_start(GTK_BOX(vbox), indexbutton, FALSE, FALSE, 0);
    g_signal_connect_swapped((gpointer) indexbutton, "clicked",
	G_CALLBACK(gtk_widget_hide), GTK_OBJECT(window));

    /* create tree model */
    model = create_model();

    /* create tree view */
    treeview = gtk_tree_view_new_with_model (model);
    gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (treeview), TRUE);
    /*
    gtk_tree_view_set_search_column (GTK_TREE_VIEW (treeview),
				     COLUMN_DESCRIPTION);
    */

    if (model)
      g_object_unref (model);

    gtk_container_add (GTK_CONTAINER (sw), treeview);

    /* add columns to the tree view */
    add_columns (GTK_TREE_VIEW (treeview));

    /* finish & show */
    gtk_window_set_default_size (GTK_WINDOW (window), 280, 250);
  }

  if (!GTK_WIDGET_VISIBLE (window))
    gtk_widget_show_all (window);
  /*
  else
    {
      gtk_widget_destroy (window);
      window = NULL;
    }
    */

  return window;
}

void display_error(const char *str)
{
  PRINTF("ERROR!\n%s\n", str);
}

void show_error(int number)
{
  static char *message = NULL;

  switch(number) {
    case E_NO_ERROR:
      break;
    case E_NO_ERROR_FAILED:
      break;
    case E_SUCCESS:
      message = strdup("Operation Successful");
      break;
    case E_STUDENT_NAME:
      message = strdup("Fix Student's Name");
      break;
    case E_FATHERS_NAME:
      message = strdup("Fix Father's Name");
      break;
    case E_MOTHERS_NAME:
      message = strdup("Fix Mother's Name");
      break;
    case E_SESSION:
      message = strdup("Fix Session");
      break;
    case E_CLASS_ROLL:
      message = strdup("Fix Class Roll Number");
      break;
    case E_BIRTH_DATE:
      message = strdup("Fix Birth Date");
      break;
    case E_TOT_LIST:
      message = strdup("Fix TOT List Serial Number");
      break;
    case E_COLLEGE_CODE:
      message = strdup("Fix College Code");
      break;
    case E_PREVIOUS_YEAR:
      message = strdup("Fix Previous Year");
      break;
    case E_PREVIOUS_ROLL:
      message = strdup("Fix Previous Roll Number");
      break;
    case E_PREVIOUS_REG:
      message = strdup("Fix Previous Registration Number");
      break;
    case E_PREVIOUS_SESS_START:
    case E_PREVIOUS_SESS_END:
      message = strdup("Fix Previous Session");
      break;
    case E_SUB_CODE:
      message = strdup("Fix Subject Code");
      break;
    case E_FILE_OPEN:
      message = strdup("Can't Open Record File (Check permission)");
      break;
    default:
      message = strdup("Unknown Error");
      break;
  }

  if (message) {
    show_message(message);
    free(message);
    message = NULL;
  }
}


/* ask for password */
void ask_pass(void)
{

  GtkWidget *ptr;
  FILE *fp;

  memset(website, 0, sizeof(website));
  memset(webaddr, 0, sizeof(webaddr));

  fp = fopen("website.txt", "r");

  if (fp) {
    fscanf(fp, "%[^:\r\n]:%[^\r\n]", website, webaddr);
    fclose(fp);
  }

  gdk_beep(); /* beep the user */
  if (!pass_window) {

    GtkWidget *entry0;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *vbox1;
    GtkWidget *hbox0;
    GtkWidget *hbox1;
    GtkWidget *hbox2;
    GtkWidget *hbox3;
    GtkWidget *label0;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *button1;
    GtkWidget *button2;



    PRINTF("Creating password window\n");
    pass_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width(GTK_CONTAINER (pass_window), 2);
    gtk_window_set_title (GTK_WINDOW(pass_window), _("Enter Password"));
    gtk_window_set_default_size(GTK_WINDOW (pass_window), 400, 200);
    gtk_window_set_position(GTK_WINDOW (pass_window), GTK_WIN_POS_CENTER);
  //  gtk_window_set_default_size (GTK_WINDOW (msg_window1), 200, 100);
    g_signal_connect(G_OBJECT(pass_window), "delete_event",
	G_CALLBACK(gtk_widget_hide_on_delete), NULL);


    vbox1 = gtk_vbox_new(FALSE, 20);
    gtk_container_add(GTK_CONTAINER(pass_window), vbox1);

    hbox0 = gtk_hbox_new(FALSE, 2);
    gtk_box_pack_start(GTK_BOX(vbox1), hbox0, TRUE, TRUE, 0);

    label0 = gtk_label_new("Server ");
    gtk_box_pack_start(GTK_BOX(hbox0), label0, FALSE, FALSE, 0);

    entry0 = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry0), 40-1);
    gtk_widget_set_size_request(entry0, 200, 30);
    gtk_box_pack_start(GTK_BOX(hbox0), entry0, TRUE, TRUE, 0);


    hbox1 = gtk_hbox_new(FALSE, 2);
    gtk_box_pack_start(GTK_BOX(vbox1), hbox1, TRUE, TRUE, 0);

    label1 = gtk_label_new("College Code  ");
    gtk_box_pack_start(GTK_BOX(hbox1), label1, FALSE, FALSE, 0);

    entry1 = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry1), 4);
    gtk_widget_set_size_request(entry1, 100, 30);
    gtk_box_pack_start(GTK_BOX(hbox1), entry1, TRUE, TRUE, 0);

    hbox2 = gtk_hbox_new(FALSE, 2);
    gtk_box_pack_start(GTK_BOX(vbox1), hbox2, TRUE, TRUE, 0);

    label2 = gtk_label_new("Password       ");
    gtk_box_pack_start(GTK_BOX(hbox2), label2, FALSE, FALSE, 0);

    entry2 = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry2), 15);
    gtk_widget_set_size_request(entry2, 100, 30);
    gtk_entry_set_visibility(GTK_ENTRY(entry2), FALSE); /* ***** */
    gtk_box_pack_start(GTK_BOX(hbox2), entry2, TRUE, TRUE, 0);

    hbox3 = gtk_hbox_new(FALSE, 2);
    gtk_box_pack_start(GTK_BOX(vbox1), hbox3, FALSE, FALSE, 0);

    button1 = gtk_button_new_from_stock("gtk-ok");
    gtk_box_pack_start(GTK_BOX(hbox3), button1, FALSE, FALSE, 0);
    GTK_WIDGET_SET_FLAGS(button1, GTK_CAN_DEFAULT);
    gtk_widget_grab_default(button1);

    button2 = gtk_button_new_from_stock("gtk-cancel");
    gtk_box_pack_start(GTK_BOX(hbox3), button2, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(button1), "clicked",
	G_CALLBACK(pass_ok_clicked), (gpointer) button1);

    g_signal_connect(G_OBJECT(button2), "clicked",
	G_CALLBACK(pass_cancel_clicked), (gpointer) button2);


    GLADE_HOOKUP_OBJECT_NO_REF (pass_window, pass_window, "pass_window");
    GLADE_HOOKUP_OBJECT (pass_window, entry0, "entry0");
    GLADE_HOOKUP_OBJECT (pass_window, entry1, "entry1");
    GLADE_HOOKUP_OBJECT (pass_window, entry2, "entry2");
    GLADE_HOOKUP_OBJECT (pass_window, button1, "button1");
    GLADE_HOOKUP_OBJECT (pass_window, button2, "button2");

    /* initialize */
    gtk_entry_set_text(GTK_ENTRY(entry0), website);
    gtk_editable_set_editable(GTK_EDITABLE(entry0), FALSE);

    gtk_entry_set_text(GTK_ENTRY(entry1), all_colcode);
    gtk_editable_set_editable(GTK_EDITABLE(entry1), FALSE); /* not editable */

  }

  if (strlen(pass) == 0) {
    ptr = lookup_widget(pass_window, "entry2");
    if (ptr)
      gtk_widget_grab_focus(ptr);
  }
  else {
    ptr = lookup_widget(pass_window, "button1");
    if (ptr)
      gtk_widget_grab_focus(ptr);
  }

  if (!GTK_WIDGET_VISIBLE(pass_window))
    gtk_widget_show_all(pass_window);
}


void pass_ok_clicked(GtkButton *button, gpointer data)
{
  GtkWidget *ptr;
  ptr = lookup_widget(GTK_WIDGET(button), "entry2");
  if (ptr) {
    strcpy(pass, gtk_entry_get_text(GTK_ENTRY(ptr)));
  }

  gtk_widget_hide(pass_window);
  /* complete pending events */
  while(gtk_events_pending())
    gtk_main_iteration();

  apply_forms(NULL);
}


void pass_cancel_clicked(GtkButton *button, gpointer data)
{
  gtk_widget_hide(pass_window);
}



void show_message(char *str)
{

  static char msg_label2_str[256];

  GtkWidget *msg_window1;
  GdkPixbuf *msg_window1_icon_pixbuf;
  GtkWidget *msg_vbox1;
  GtkWidget *msg_hbox1;
  GtkWidget *msg_frame1;
//  GtkWidget *msg_fixed1;
  GtkWidget *msg_image1;
  GtkWidget *msg_label2;
  GtkWidget *msg_label1;
  GtkWidget *msg_button1;
  GtkTooltips *msg_tooltips;

  GtkWidget *button_hbox;

  gdk_beep(); /* beep the user */

  if (!msgwin) {

    if (!strlen(str))
      return;

    msg_tooltips = gtk_tooltips_new ();

    PRINTF("Creating message window\n");
//    fflush(stdout);
    msgwin = msg_window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);

//    gtk_widget_set_name (msg_window1, "msg_window1");
    gtk_container_set_border_width (GTK_CONTAINER (msg_window1), 2);
    gtk_tooltips_set_tip (msg_tooltips, msg_window1, _("Information"), NULL);
    gtk_window_set_title (GTK_WINDOW (msg_window1), _("Message Window"));
    gtk_window_set_position (GTK_WINDOW (msg_window1), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size (GTK_WINDOW (msg_window1), 200, 100);
    msg_window1_icon_pixbuf = create_pixbuf ("bug-buddy.png");
    if (msg_window1_icon_pixbuf)
      {
	gtk_window_set_icon (GTK_WINDOW (msg_window1), msg_window1_icon_pixbuf);
	gdk_pixbuf_unref (msg_window1_icon_pixbuf);
      }

    msg_vbox1 = gtk_vbox_new (FALSE, 0);
//    gtk_widget_set_name (msg_vbox1, "msg_vbox1");
//    gtk_widget_show (msg_vbox1);
    gtk_container_add (GTK_CONTAINER (msg_window1), msg_vbox1);

    msg_frame1 = gtk_frame_new (NULL);
//    gtk_widget_set_name (msg_frame1, "msg_frame1");
//    gtk_widget_show (msg_frame1);
    gtk_box_pack_start (GTK_BOX (msg_vbox1), msg_frame1, TRUE, TRUE, 0);
    gtk_frame_set_label_align (GTK_FRAME (msg_frame1), 0.5, 0.5);

    msg_hbox1 = gtk_hbox_new(FALSE, 0);
    gtk_widget_show(msg_hbox1);
    gtk_container_add(GTK_CONTAINER(msg_frame1), msg_hbox1);

    msg_image1 = gtk_image_new_from_stock ("gtk-dialog-info", GTK_ICON_SIZE_DIALOG);
    gtk_box_pack_start(GTK_BOX(msg_hbox1), msg_image1, FALSE, FALSE, 0);

    /*
    msg_fixed1 = gtk_fixed_new ();
    gtk_widget_set_name (msg_fixed1, "msg_fixed1");
    gtk_widget_show (msg_fixed1);
    gtk_container_add (GTK_CONTAINER (msg_frame1), msg_fixed1);
    */

//    gtk_widget_set_name (msg_image1, "msg_image1");
//    gtk_widget_show (msg_image1);
//    gtk_fixed_put (GTK_FIXED (msg_fixed1), msg_image1, 40, 40);
//    gtk_widget_set_size_request (msg_image1, 60, 60);

    sprintf(msg_label2_str, "\n     %s     \n", str);
//    sprintf(msg_label2_str, "\n     <span foreground=\"#C16C6C\">%s</span>     \n", str);
    msglabel = msg_label2 = gtk_label_new (_(msg_label2_str));
//    gtk_widget_set_name (msg_label2, "msg_label2");
//    gtk_widget_show (msg_label2);
//    gtk_fixed_put (GTK_FIXED (msg_fixed1), msg_label2, 112, 0);
//    gtk_widget_set_size_request (msg_label2, 290, 240);
    gtk_box_pack_start(GTK_BOX(msg_hbox1), msg_label2, TRUE, TRUE, 0);
    gtk_label_set_use_markup (GTK_LABEL (msg_label2), TRUE);
    gtk_label_set_justify (GTK_LABEL (msg_label2), GTK_JUSTIFY_LEFT);

    msg_label1 = gtk_label_new (_("<span foreground=\"maroon\">Message</span>"));
//    gtk_widget_set_name (msg_label1, "msg_label1");
//    gtk_widget_show (msg_label1);
    gtk_frame_set_label_widget (GTK_FRAME (msg_frame1), msg_label1);
    gtk_label_set_use_markup (GTK_LABEL (msg_label1), TRUE);
    gtk_label_set_justify (GTK_LABEL (msg_label1), GTK_JUSTIFY_CENTER);

    button_hbox = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(msg_vbox1), button_hbox, FALSE, FALSE, 0);

    msg_button1 = gtk_button_new_from_stock ("gtk-ok");
//    gtk_widget_set_name (msg_button1, "msg_button1");
//    gtk_widget_show (msg_button1);
    gtk_box_pack_start (GTK_BOX (button_hbox), msg_button1, FALSE, FALSE, 0);

    g_signal_connect (GTK_OBJECT(msg_window1), "delete_event",
		      G_CALLBACK (gtk_widget_hide_on_delete),
		      NULL);
    g_signal_connect_swapped ((gpointer) msg_button1, "clicked",
			      G_CALLBACK (gtk_widget_hide),
			      GTK_OBJECT (msg_window1));

    /* Store pointers to all widgets, for use by lookup_widget(). */
    /*
    GLADE_HOOKUP_OBJECT_NO_REF (msg_window1, msg_window1, "msg_window1");
    GLADE_HOOKUP_OBJECT (msg_window1, msg_vbox1, "msg_vbox1");
    GLADE_HOOKUP_OBJECT (msg_window1, msg_frame1, "msg_frame1");
    GLADE_HOOKUP_OBJECT (msg_window1, msg_fixed1, "msg_fixed1");
    GLADE_HOOKUP_OBJECT (msg_window1, msg_image1, "msg_image1");
    GLADE_HOOKUP_OBJECT (msg_window1, msg_label2, "msg_label2");
    GLADE_HOOKUP_OBJECT (msg_window1, msg_label1, "msg_label1");
    GLADE_HOOKUP_OBJECT (msg_window1, msg_button1, "msg_button1");
    GLADE_HOOKUP_OBJECT_NO_REF (msg_window1, msg_tooltips, "msg_tooltips");
    */

    gtk_widget_show_all(msgwin);
  }
  else { /* window already created */
//    sprintf(msg_label2_str, "\n     <span foreground=\"#C16C6C\">%s</span>     \n", str);
    sprintf(msg_label2_str, "\n     %s     \n", str);
    gtk_label_set_text(GTK_LABEL(msglabel), _(msg_label2_str));
    gtk_label_set_use_markup (GTK_LABEL (msglabel), TRUE);
  }
  if (!GTK_WIDGET_VISIBLE(msgwin)) {
    gtk_widget_show(msgwin);
  }
}



/* pass is simple text right now
 * but will be sha1 encrypted before sending to server
 */
int authenticate(const char *username, const char *pass)
{
  static char buffer[80];
//  EVP_MD_CTX mdctx; /* message digest context */
//  const EVP_MD *md;
//  static unsigned char md_value[EVP_MAX_MD_SIZE+1];
//  int md_len;
  SHA_CTX shactx;
  static unsigned char md_value[SHA_DIGEST_LENGTH];

  int nwrite;
  int ret;
  int errval;

  GtkWidget *ptr;
//  GString *msg = NULL;

//  PRINTF("EVP_MAX_MD_SIZE = %d\n", EVP_MAX_MD_SIZE);

  if (!username || strlen(username) != 4) { /* college code */
    show_message("College code must be 4 digits");
    return 1;
  }
  /*
  if (!pass || strlen(pass) < 6) {
    show_message("Password must be at least 6 characters");
    return 2;
  }
  */


  ptr = lookup_widget(online_transfer_win, "label1");
  if (ptr) {
    strcpy(error_msg, gtk_label_get_label(GTK_LABEL(ptr)));
    strcat(error_msg, "\nSending magic ... ");
    gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
  }

  while(gtk_events_pending()) /* so that user can see progress */
    gtk_main_iteration();

//  sleep(3);
  nwrite = SHA_DIGEST_LENGTH;
  PRINTF("Sending magic ... ");
  if ((ret = writen(sockfd, magic, nwrite)) != nwrite) {
    PRINTF("[FAILED]\n");
    strcat(error_msg, "[FAILED]");
    gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
    while(gtk_events_pending()) /* so that user can see progress */
      gtk_main_iteration();

    return 1;
  }
  PRINTF("[OK]\n");
  strcat(error_msg, "[  OK  ]\nWaiting for feedback ... ");
  gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
  while(gtk_events_pending()) /* so that user can see progress */
    gtk_main_iteration();

//  sleep(3);
  PRINTF("Waiting for feedback ... ");
  if ((ret = readline(sockfd, buffer, sizeof(buffer))) != 4) {
    strcat(error_msg, "[FAILED]\n");
    gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
    while(gtk_events_pending())
      gtk_main_iteration();
    PRINTF("[FAILED]\n");
    return 1;
  }
  PRINTF("[  OK  ]\n");

  if (!strncmp(buffer, "500\n", 4)) {
    strcat(error_msg, "[  OK  ]\n");
    gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
    while(gtk_events_pending()) /* so that user can see progress */
      gtk_main_iteration();
    PRINTF("Magic OK\n");
  }
  else {
    strcat(error_msg, "[FAILED]\n");
    gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
    while(gtk_events_pending()) /* so that user can see progress */
      gtk_main_iteration();
    PRINTF("Magic failure\n");
    return 1;
  }

  /* first send username */
  sprintf(buffer, "USER %s\n", username);
  nwrite = strlen(buffer);
  PRINTF("Sending college code ... ");
  strcat(error_msg, "Sending college code ... ");
  gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
  while(gtk_events_pending()) /* so that user can see progress */
    gtk_main_iteration();

//  sleep(3);

  if ((ret = writen(sockfd, buffer, nwrite)) != nwrite) {
    strcat(error_msg, "[FAILED]\n");
    gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
    while(gtk_events_pending()) /* so that user can see progress */
      gtk_main_iteration();
    PRINTF("[FAILED]\n");
    return 1;
  }
  PRINTF("[OK]\n");

  PRINTF("Waiting for reply ...");
  strcat(error_msg, "[  OK  ]\nWaiting for reply ... ");
  gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
  while(gtk_events_pending()) /* so that user can see progress */
    gtk_main_iteration();
//  sleep(3);
  if ((ret = readline(sockfd, buffer, MAXLINE)) < 4) {
    strcat(error_msg, "[FAILED]\n");
    gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
    while(gtk_events_pending()) /* so that user can see progress */
      gtk_main_iteration();
    PRINTF("[FAILED]\n");
    return 1;
  }
  PRINTF("[  OK  ]\n");
  errval = strtol(buffer, (char **) NULL, 10);
  switch(errval) {
    case E_NO_COLLEGE:
      PRINTF("E_NO_COLLEGE\n");
      strcat(error_msg, "<span foreground=\"red\">No such college code</span>");
      gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
      while(gtk_events_pending()) /* so that user can see progress */
	gtk_main_iteration();
//      show_message("No such college code in server");
      return 1;
      break;
    case E_COLLEGE_DISABLED:
      PRINTF("E_COLLEGE_DISABLED\n");
      strcat(error_msg, "<span foreground=\"red\">College disabled</span>");
      gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
      while(gtk_events_pending()) /* so that user can see progress */
	gtk_main_iteration();
//      show_message("College Disabled");
      return 1;
      break;
    case E_TIME_NOSTART:
      PRINTF("E_TIME_NOSTART\n");
      strcat(error_msg, "[  OK  ]\nYour can send forms from : <span foreground=\"red\">");
      strcat(error_msg, buffer+3);
      strcat(error_msg, "</span>");
      gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
      while(gtk_events_pending()) /* so that user can see progress */
	gtk_main_iteration();
      return 1;
      break;
    case E_TIME_EXPIRED:
      PRINTF("E_TIME_EXPIRED\n");
      strcat(error_msg, "[  OK  ]\nYour form submission date has expired on : <span foreground=\"red\">");
      strcat(error_msg, buffer+3);
      strcat(error_msg, "</span>");
      gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
      while(gtk_events_pending()) /* so that user can see progress */
	gtk_main_iteration();
      return 1;
      break;
    case 500:
      PRINTF("[  OK  ]\n");
      strcat(error_msg, "[  OK  ]\nYou can submit form between : <span foreground=\"#6D3C78\"><b>");
      strcat(error_msg, buffer+3);
      strcat(error_msg, "</b></span>");
      break;
    default:
      PRINTF("Unknown error\n");
      PRINTF("SERVER: %d\n", errval);
      strcat(error_msg, "<span foreground=\"red\">Unknown error</span>");
      gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
      while(gtk_events_pending()) /* so that user can see progress */
	gtk_main_iteration();
      return 1;
      break;
  }

  gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
  while(gtk_events_pending()) /* so that user can see progress */
    gtk_main_iteration();
//  sleep(3);

  /* initializing password */
//  PRINTF("Initializing digest\n");
//  OpenSSL_add_all_digests();

//  md = EVP_get_digestbyname(DIGEST);

//  if (!md) {
//    PRINTF("Unknown message digest %s\n", DIGEST);
//    return 3;
//  }

  memset(md_value, 0, sizeof(md_value));
  strcpy(buffer, "PASS ");

  /*
  EVP_DigestInit(&mdctx, md);
  EVP_DigestUpdate(&mdctx, pass, strlen(pass));
  EVP_DigestFinal(&mdctx, md_value, &md_len);
  memcpy(buffer+5, md_value, md_len);
  */
//  PRINTF("Digest finalization complete\n");

//  PRINTF("Password is %d bytes long\n", md_len);

  /* be careful when sending password
   * server knows the length of the password
   * and encrypted password may contain embedded newline
   * so don't send any extra newline
   * and server will read just the characters of length of the password
   */
  SHA1_Init(&shactx);
  SHA1_Update(&shactx, pass, strlen(pass));
  SHA1_Final(md_value, &shactx);
  memcpy(buffer+5, md_value, SHA_DIGEST_LENGTH); /* copy password to buffer */

  nwrite = 5+SHA_DIGEST_LENGTH;
//  PRINTF("buff = %s\n", buffer);
//  PRINTF("len = %d\n", nwrite);
  PRINTF("Writing password to socket ... ");
  strcat(error_msg, "\nWriting password to socket ... ");
  gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
  while(gtk_events_pending()) /* so that user can see progress */
    gtk_main_iteration();
//  sleep(3);

  if ((ret = writen(sockfd, buffer, nwrite)) != nwrite) {
    strcat(error_msg, "[FAILED]");
    gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
    while(gtk_events_pending()) /* so that user can see progress */
      gtk_main_iteration();
//    sleep(3);

    PRINTF("[FAILED]\n");
    return 1;
  }
  strcat(error_msg, "[  OK  ]\nWaiting for reply ... ");
  gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
  while(gtk_events_pending()) /* so that user can see progress */
    gtk_main_iteration();
//  sleep(3);

  PRINTF("[  OK  ]\n");
  PRINTF("Waiting for reply ...");
  if ((ret = readline(sockfd, buffer, MAXLINE)) != 4) {
    PRINTF("[FAILED]\n");
    return 1;
  }
  strcat(error_msg, "[  OK  ]");
  gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
  while(gtk_events_pending()) /* so that user can see progress */
    gtk_main_iteration();
//  sleep(3);

  PRINTF("[  OK  ]\n");
  errval = strtol(buffer, (char **) NULL, 10);
  switch(errval) {
    case E_INVALID_PASSWORD:
      PRINTF("Invalid password\n");
      strcat(error_msg, "\n<span foreground=\"red\"><b>Invalid Password</b></span>");
      gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
      while(gtk_events_pending()) /* so that user can see progress */
	gtk_main_iteration();
//      sleep(3);
//      show_message("Invalid Password");
      return 1;
      break;
    default:
//      strcat(error_msg, "\n[  OK  ]");
      PRINTF("SERVER: %d\n", errval);
      break;
  }

//  strcat(error_msg, "[  OK  ]");

  return 0;
}



void load_college_info(const char *filename, GtkWidget *code, GtkWidget *name)
{
  FILE *fp = NULL;
  char code_str[5];
  char name_str[61];
//  char error_msg[80];

  fp = fopen(filename, "r");
  if (!fp) {
    PRINTF("fopen: %s: %s\n", filename, strerror(errno));
    return;
  }
  else {
    fscanf(fp, "%s %[^\r\n]", code_str, name_str);
    fclose(fp);

    PRINTF("College Code : %s\n", code_str);
    PRINTF("College Name : %s\n", name_str);

    gtk_entry_set_text(GTK_ENTRY(code), code_str);
    gtk_entry_set_text(GTK_ENTRY(name), name_str);
    /* now make the boxes read-only */
    /* gtk_entry_set_editable() is deprecated.
     * gtk_editable_set_editable() is preferred
     */
    gtk_editable_set_editable(GTK_EDITABLE(code), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(name), FALSE);

  }
}


/* load common subject for a specific group (determined by filename) */
/* caller should call this routine with the starting
 * widget it wants to be filled
 * this function returns number of entries filled (starting from 1)
 */
int load_sub_common(const char *filename, GtkWidget *ent[])
{
//  GtkWidget *entry;
//  char error_msg[80];
  FILE *fp = NULL;
  char sub_code_str[4];
  int codeval;
  int i = 0;
  int ret;

  PRINTF("load_sub_common : ");

  fp = fopen(filename, "r");
  if (!fp) {
    PRINTF("load_sub_common: fopen: %s: %s\n", filename, strerror(errno));
    return 0;
  }
  else {
    while (ent[i] && fscanf(fp, "%d", &codeval) == 1) {
      sprintf(sub_code_str, "%03d", codeval);
      PRINTF("sub_code_str => %s\n", sub_code_str);
      gtk_entry_set_text(GTK_ENTRY(ent[i]), sub_code_str);
      gtk_editable_set_editable(GTK_EDITABLE(ent[i]), FALSE);
      set_sub_tip(ent[i], strtol(sub_code_str, NULL, 10));
      i++;
    }
    fclose(fp);
  }

  ret = i;

  /* make rest of the entries editable */
  while(ent[i]) {
    gtk_editable_set_editable(GTK_EDITABLE(ent[i]), TRUE);
    i++;
  }

  return ret;
}


/* clears rest of the subject entries starting from ent */
void clear_sub_entry(GtkWidget *ent[])
{
  int i = 0;

  while(ent[i]) { /* go on until NULL */
    gtk_entry_set_text(GTK_ENTRY(ent[i]), "");
    i++;
  }
}

/* converts 'str' to uppercase and returns the result */
/* buffer size is 80;
 * so, never try to use this function if the input string
 * is supposed to be greater than the buffer size
 */
/* this routine uses its own static buffer
 * so, don't try to free it
 */
/* this routine is not thread safe */
char *strupper(const char *str)
{
#ifdef BUFLEN
  static char buf[BUFLEN];
#else
  static char buf[80];
#endif

  register int i;

  if (str == NULL)
    return NULL;

  for (i=0; *(str+i) != '\0'; i++) {
    *(buf+i) = (char) toupper(*(str+i));
  }
  *(buf+i) = (char) '\0';

  return buf;
}

GHashTable *create_sub_hash_table(const char *filename)
{
//  GHashTable *htable = NULL;
  FILE *fp;
  int code;
  char name[40];
  int *codep; /* pointer to the code */
  char *namep; /* pointer to the name */

  if (subhtable) { /* if already created, return it */
    PRINTF("Subject code hash table already created; returning\n");
    return subhtable;
  }

  if (!filename || !strlen(filename)) /* filename not supplied */
    return subhtable; /* subhtable == NULL */

  fp = fopen(filename, "r");
  if (!fp) {
    PRINTF(filename);
    PRINTF("fopen: %s: %s\n", filename, strerror(errno));
    return subhtable; /* subhtable == NULL */
  }

  PRINTF("Creating subject code hash table\n");
  subhtable = g_hash_table_new_full(g_int_hash,
      g_int_equal,
      free,  /* standard free() routine */
      free); /* standard free() routine */

  memset(name, 0, sizeof(name));

  while(fscanf(fp, "%d%[^\r\n]", &code, name) > 0) { /* while input available */
    codep = (int *) malloc(sizeof(int));
    *codep = code;
    namep = strdup(name);

    PRINTF("Adding %d => %s\n", *codep, namep);
    g_hash_table_insert(subhtable, codep, namep);
  }

  fclose(fp);

  return subhtable;
}


/* college code hash table */
GHashTable *create_col_hash_table(const char *filename)
{
//  GHashTable *htable = NULL;
  FILE *fp;
  int code;
  char name[71]; /* college name */
  int *codep; /* pointer to the code */
  char *namep; /* pointer to the name */

  if (colhtable) { /* if already created, return it */
    PRINTF("College code hash table already created; returning\n");
    return colhtable;
  }

  if (!filename || !strlen(filename)) /* filename not supplied */
    return colhtable; /* subhtable == NULL */

  fp = fopen(filename, "r");
  if (!fp) {
    PRINTF(filename);
    PRINTF("fopen: %s: %s\n", filename, strerror(errno));
    return colhtable; /* subhtable == NULL */
  }

//  PRINTF("Creating college code hash table\n");
  colhtable = g_hash_table_new_full(g_int_hash,
      g_int_equal,
      free,  /* standard free() routine */
      free); /* standard free() routine */

  memset(name, 0, sizeof(name));

  while(fscanf(fp, "%d %[^\r\n]", &code, name) > 0) { /* while input available */
    codep = (int *) malloc(sizeof(int));
    *codep = code;
    namep = strdup(name);

    PRINTF("Adding %d => %s\n", *codep, namep);
    g_hash_table_insert(colhtable, codep, namep);
  }

  fclose(fp);

//  PRINTF("Hash Complete\n");
  return colhtable;
}

void remove_col_hash_table(void)
{
  g_hash_table_destroy(colhtable);
}

/* fill up college code and name */
void update_college_code_name(GtkEntry *code, GtkEntry *name)
{
  /*
  gtk_entry_set_text(code, col_code);
  gtk_entry_set_text(name, col_name);
  */
}

/* fill up college code and name */
void update_session(GtkEntry *entry)
{
  /*
  gtk_entry_set_text(entry, all_session);
  */
}

char *lookup_college_by_code(const char *code)
{
  int codeval;

  create_col_hash_table(COLLEGEFILE);
  if (!colhtable) {
    PRINTF("colhtable is null\n");
    return NULL;
  }

  PRINTF("lookup_college_by_code : ");
  codeval = (int) strtol(code, (char **) NULL, 10);
  PRINTF("%d\n", codeval);
  return ((char *) g_hash_table_lookup(colhtable, &codeval));
}


GtkWidget*
query (void)
{
  GtkWidget *window1;
  GtkWidget *frame1;
  GtkWidget *alignment1;
  GtkWidget *fixed1;
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *hseparator1;
  GtkWidget *button1;
  GtkWidget *label2;
  GtkWidget *label3;
  GtkWidget *label4;
  GtkWidget *label1;

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window1), "Query Window");
  gtk_window_set_resizable (GTK_WINDOW (window1), FALSE);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_show (frame1);
  gtk_container_add (GTK_CONTAINER (window1), frame1);
  gtk_widget_set_size_request (frame1, 400, 259);

  alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (frame1), alignment1);

  fixed1 = gtk_fixed_new ();
  gtk_widget_show (fixed1);
  gtk_container_add (GTK_CONTAINER (alignment1), fixed1);

  entry1 = gtk_entry_new (); /* college code */
  gtk_widget_show (entry1);
  gtk_fixed_put (GTK_FIXED (fixed1), entry1, 192, 72);
  gtk_widget_set_size_request (entry1, 158, 40);
  gtk_entry_set_max_length (GTK_ENTRY (entry1), 4);
  gtk_editable_set_editable(GTK_EDITABLE(entry1), FALSE);

  entry2 = gtk_entry_new ();
  gtk_widget_show (entry2);
  gtk_fixed_put (GTK_FIXED (fixed1), entry2, 192, 8);
  gtk_entry_set_max_length (GTK_ENTRY (entry2), 9); /* nnnn-nnnn */
  gtk_widget_set_size_request (entry2, 160, 40);

  hseparator1 = gtk_hseparator_new ();
  gtk_widget_show (hseparator1);
  gtk_fixed_put (GTK_FIXED (fixed1), hseparator1, 0, 168);
  gtk_widget_set_size_request (hseparator1, 368, 16);

  button1 = gtk_button_new_from_stock ("gtk-ok");
  /* don't show the button at first */
//  gtk_widget_show (button1);
  gtk_fixed_put (GTK_FIXED (fixed1), button1, 128, 192);
  gtk_widget_set_size_request (button1, 96, 32);
  gtk_container_set_border_width (GTK_CONTAINER (button1), 1);
  /* the button can be the default button */
  GTK_WIDGET_SET_FLAGS(button1, GTK_CAN_DEFAULT);

  label2 = gtk_label_new ("Enter Institution Code");
  gtk_widget_show (label2);
  gtk_fixed_put (GTK_FIXED (fixed1), label2, 8, 72);
  gtk_widget_set_size_request (label2, 176, 40);

  label3 = gtk_label_new ("Enter Session");
  gtk_widget_show (label3);
  gtk_fixed_put (GTK_FIXED (fixed1), label3, 8, 8);
  gtk_widget_set_size_request (label3, 144, 40);

  label4 = gtk_label_new ("");
  gtk_fixed_put (GTK_FIXED (fixed1), label4, 0, 128);
  gtk_widget_set_size_request (label4, 368, 25);
  gtk_label_set_justify (GTK_LABEL (label4), GTK_JUSTIFY_CENTER);
  gtk_label_set_use_markup (GTK_LABEL (label4), TRUE);

  label1 = gtk_label_new ("<b>Query</b>");
  gtk_widget_show (label1);
  gtk_frame_set_label_widget (GTK_FRAME (frame1), label1);
  gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);

  g_signal_connect_swapped ((gpointer) window1, "delete_event",
                            G_CALLBACK (on_q_window_delete),
                            GTK_OBJECT (window1));
  g_signal_connect_swapped ((gpointer) entry1, "changed",
                            G_CALLBACK (q_col_entry_changed),
                            GTK_OBJECT (entry1));
  g_signal_connect_swapped ((gpointer) entry2, "changed",
                            G_CALLBACK (q_sess_entry_changed),
                            GTK_OBJECT (entry2));
  g_signal_connect_swapped ((gpointer) button1, "clicked",
                            G_CALLBACK (q_okbutton_clicked),
                            GTK_OBJECT (button1));

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (window1, window1, "window1");
  GLADE_HOOKUP_OBJECT (window1, frame1, "frame1");
  GLADE_HOOKUP_OBJECT (window1, alignment1, "alignment1");
  GLADE_HOOKUP_OBJECT (window1, fixed1, "fixed1");
  GLADE_HOOKUP_OBJECT (window1, entry1, "entry1");
  GLADE_HOOKUP_OBJECT (window1, entry2, "entry2");
  GLADE_HOOKUP_OBJECT (window1, hseparator1, "hseparator1");
  GLADE_HOOKUP_OBJECT (window1, button1, "button1");
  GLADE_HOOKUP_OBJECT (window1, label2, "label2");
  GLADE_HOOKUP_OBJECT (window1, label3, "label3");
  GLADE_HOOKUP_OBJECT (window1, label4, "label4");
  GLADE_HOOKUP_OBJECT (window1, label1, "label1");

  return window1;
}

gboolean on_q_window_delete(GtkWidget *widget,
    GdkEvent *event,
    gpointer data)
{
  gtk_main_quit();
  return FALSE;
}


void q_col_entry_changed(GtkEditable *entry, gpointer data)
{
  char *codestr;
  char *codeval;
  GtkWidget *ptr;
  size_t i;

  PRINTF("q_col_entry_changed : ");
  codestr = strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
  PRINTF("%s\n", codestr);

  ptr = lookup_widget(GTK_WIDGET(entry), "label4");
  if (!ptr) {
    free(codestr);
    return;
  }

  if (!GTK_WIDGET_VISIBLE(ptr))
    gtk_widget_show(ptr);

  i = strspn(codestr, ACCEPTNUMBERS);
  *(codestr+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(entry), codestr);

  codeval = lookup_college_by_code(codestr);
  if (!codeval) {
    gtk_label_set_text(GTK_LABEL(ptr), "No matching college name");
    gtk_widget_hide(lookup_widget(GTK_WIDGET(entry), "button1"));
  }
  else {
    /* use error_msg for other purposes; save memory */
    sprintf(error_msg, "<span foreground=\"maroon\"><b>%s</b></span>", codeval);
    gtk_label_set_markup(GTK_LABEL(ptr), error_msg);
    ptr = lookup_widget(GTK_WIDGET(entry), "button1");
    gtk_widget_show(ptr);
    /* make the button by default grabbed */
//    GTK_WIDGET_SET_FLAGS(ptr, GTK_CAN_DEFAULT);
//    gtk_widget_grab_default(ptr);
    gtk_widget_grab_focus(ptr);
  }

  free(codestr);

}

void q_sess_entry_changed(GtkEditable *entry, gpointer data)
{
  char *colval;
  GtkWidget *ptr;
  size_t i;
  /* if previous length of the entry is greater than the current
   * length, don't fix entry */


  static char buf[10];

  char *val;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
  i = strspn(val, ACCEPTSESSION);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(entry), val);
  if (prevlen < strlen(val))
    prevlen = strlen(val); /* for the first time */

  /* if once backspace is used by user,
   * don't do auto complete
   * probably user wants to enter values
   * manually
   */


  if (i < 4) {
    ptr = lookup_widget(GTK_WIDGET(entry), "entry1");
    if (ptr)
      gtk_editable_set_editable(GTK_EDITABLE(ptr), FALSE);
    ptr = lookup_widget(GTK_WIDGET(entry), "button1");
    if (ptr)
      gtk_widget_hide(ptr);
    free(val);
    return;
  }
  else if ((i == 4) && (prevlen <= i)) { /* let the user edit entry */
    i = (int) strtol(val, (char **) NULL, 10);
    sprintf(buf, "%04d-%04d", (int) i, (int) (i+1));
    gtk_entry_set_text(GTK_ENTRY(entry), buf);
    i = 9; /* grab next entry */
  }

  ptr = lookup_widget(GTK_WIDGET(entry), "entry1");
  if (ptr) {
    gtk_editable_set_editable(GTK_EDITABLE(ptr), TRUE);
    if (i == 9)
      gtk_widget_grab_focus(ptr);
  }

  colval = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));

  if (lookup_college_by_code(colval)) {
    ptr = lookup_widget(GTK_WIDGET(entry), "button1");
    if (ptr && !GTK_WIDGET_VISIBLE(ptr))
      gtk_widget_show(ptr);
  }

  free(val);
  free(colval);
}



/* lookup a subject name by its code from a previously created hash */
/* this routine uses global 'subhtable' (if not NULL) */
const char *lookup_subject_by_code(unsigned int key)
{
  const char *filename = SUBCODEFILE;
  unsigned int code = key;

  if (!subhtable) {
    create_sub_hash_table(filename);
  }

  PRINTF("lookup subject of code %d\n", key);

  return ((const char *) g_hash_table_lookup(subhtable, &code));
}

/* sets tips to the specified widget */
/* if code is zero, set '3 digits' */
void set_sub_tip(GtkWidget *widget, unsigned int code)
{
  GtkTooltips *tips = NULL;
  const char *subname = NULL;

  tips = GTK_TOOLTIPS(lookup_widget(widget, "tooltips"));

  if (!tips) {
    PRINTF("Creating tooltips for subject code\n");
    tips = gtk_tooltips_new();
  }


  if (code == 0) { /* specially handle this */
    gtk_tooltips_set_tip(tips, widget, "3 digits", NULL);
    return;
  }

  subname = lookup_subject_by_code(code);
  if (subname) {
    PRINTF("Subject name found : %d => %s\n", code, subname);
    gtk_tooltips_set_tip(tips, widget, subname, NULL);
  }
  else {
    PRINTF("Subject name NOT found for %d\n", code);
    gtk_tooltips_set_tip(tips, widget, "Unknown", NULL);
  }
}

void update_gender(GtkEntry *entry)
{
  GtkWidget *ptr;
  char *name;

  ptr = lookup_widget(GTK_WIDGET(entry), "combo_entry8");

  if (!ptr) /* not found, return */
    return;

  PRINTF("combo_entry8 found\n");

  name = strdup(gtk_entry_get_text(entry));
  if (!strncasecmp(name, "MOHAMMAD", 8)) {
    gtk_entry_set_text(GTK_ENTRY(ptr), "Male");
  }
  else if (!strncasecmp(name, "MD ", 3)) {
    gtk_entry_set_text(GTK_ENTRY(ptr), "Male");
  }
  else if (!strncasecmp(name, "MD.", 3)) {
    gtk_entry_set_text(GTK_ENTRY(ptr), "Male");
  }
  else if (!strncasecmp(name, "MOSAMMAT", 8)) {
    gtk_entry_set_text(GTK_ENTRY(ptr), "Female");
  }
  else if (!strncasecmp(name, "MST ", 4)) {
    gtk_entry_set_text(GTK_ENTRY(ptr), "Female");
  }
  else if (!strncasecmp(name, "MST.", 4)) {
    gtk_entry_set_text(GTK_ENTRY(ptr), "Female");
  }

  free(name);
}


/* reads a line from a socket */
/* NOT thread-safe */
#ifdef _WIN32
static ssize_t my_read(SOCKET fd, char *ptr)
#else
static ssize_t my_read(int fd, char *ptr)
#endif
{
  static int read_cnt = 0;
  static char *read_ptr;
  static char read_buf[MAXLINE];


//  PRINTF("***debug: IN my_read: ***\n");

  if (read_cnt <= 0) {
again:
#ifdef _WIN32
    if ((read_cnt = recv(fd, read_buf, sizeof(read_buf), 0)) < 0) {
#else
    if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
#endif
      if (errno == EINTR)
	goto again;
      return (-1);
    } else if (read_cnt == 0)
      return (0);
    read_ptr = read_buf;
  }
  read_cnt--;
  *ptr = *read_ptr++;

//  PRINTF("***debug: OUT my_read: ***\n");

  return (1);
}

#ifdef _WIN32
ssize_t readline(SOCKET fd, void *vptr, size_t maxlen)
#else
ssize_t readline(int fd, void *vptr, size_t maxlen)
#endif
{
  int n, rc;
  char c, *ptr;

//  PRINTF("***debug: IN readline: ***\n");

  ptr = vptr;
  for (n = 1; n < maxlen; n++) {
    if ((rc = my_read(fd, &c)) == 1) {
      *ptr++ = c;
      if (c == '\n')
	break; /* newline is stored, like fgets() */
    } else if (rc == 0) {
      if (n == 1)
	return (0); /* EOF, no data */
      else
	break; /* EOF, some data was read */
    } else
	return (-1); /* error, errno set by read() */
  }
  *ptr = 0; /* null terminate like fgets() */

//  PRINTF("***debug: OUT readline: ***\n");

  return (n);
}

#ifdef _WIN32
ssize_t readn(SOCKET fd, void *vptr, size_t n)
#else
ssize_t readn(int fd, void *vptr, size_t n)
#endif
{
  size_t nleft;
  ssize_t nread;
  char *ptr;

  ptr = vptr;
  nleft = n;
  while (nleft > 0) {
#ifdef _WIN32
    if ((nread = recv(fd, ptr, nleft, 0)) < 0) {
#else
    if ((nread = read(fd, ptr, nleft)) < 0) {
#endif
      if (errno == EINTR)
	nread = 0; /* call read() again */
      else
	return (-1);
    } else if (nread == 0)
      break; /* EOF */
    nleft -= nread;
    ptr += nread;
  }
  return (n - nleft);
}

#ifdef _WIN32
ssize_t writen(SOCKET fd, const void *vptr, size_t n)
#else
ssize_t writen(int fd, const void *vptr, size_t n)
#endif
{
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;

  ptr = vptr;
  nleft = n;
  while (nleft > 0) {
#ifdef _WIN32
    if ((nwritten = send(fd, ptr, nleft, 0)) <= 0) {
#else
    if ((nwritten = write(fd, ptr, nleft)) <= 0) {
#endif
      if (errno == EINTR)
	nwritten = 0; /* call write() again */
      else
	return (-1); /* error */
    }
    nleft -= nwritten;
    ptr += nwritten;
  }
  return (n);
}



GtkWidget* view_syllabus (void)
{
  GtkWidget *window1;
  GtkWidget *frame1;
  GtkWidget *alignment1;
  GtkWidget *scrolledwindow1;
  GtkWidget *viewport1;
  GtkWidget *image1;
  GtkWidget *label1;

  PRINTF("Creating syllabus window\n");

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (window1), 2);
  gtk_window_set_title (GTK_WINDOW (window1), "Syllabus");
  gtk_window_set_default_size(GTK_WINDOW(window1), 700, 500);
  gtk_window_set_position (GTK_WINDOW (window1), GTK_WIN_POS_CENTER);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_show (frame1);
  gtk_container_add (GTK_CONTAINER (window1), frame1);
  gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_NONE);

  alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (frame1), alignment1);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow1);
  gtk_container_add (GTK_CONTAINER (alignment1), scrolledwindow1);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_SHADOW_ETCHED_IN);

  viewport1 = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (viewport1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), viewport1);

  image1 = create_pixmap (window1, "syllabus.jpg");
  gtk_widget_show (image1);
  gtk_container_add (GTK_CONTAINER (viewport1), image1);

  label1 = gtk_label_new ("<b>Syllabus</b>");
  gtk_widget_show (label1);
  gtk_frame_set_label_widget (GTK_FRAME (frame1), label1);
  gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);

  g_signal_connect_swapped ((gpointer) window1, "delete_event",
                            G_CALLBACK (gtk_widget_hide),
                            GTK_OBJECT (window1));

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (window1, window1, "window1");
  GLADE_HOOKUP_OBJECT (window1, frame1, "frame1");
  GLADE_HOOKUP_OBJECT (window1, alignment1, "alignment1");
  GLADE_HOOKUP_OBJECT (window1, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (window1, viewport1, "viewport1");
  GLADE_HOOKUP_OBJECT (window1, image1, "image1");
  GLADE_HOOKUP_OBJECT (window1, label1, "label1");

  return window1;
}

GtkWidget* confirm_dialog (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *fixed1;
  GtkWidget *label1;
  GtkWidget *image1;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), "Confirm Exit");
  gtk_window_set_resizable (GTK_WINDOW (dialog1), FALSE);
  gtk_window_set_type_hint (GTK_WINDOW (dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  fixed1 = gtk_fixed_new ();
  gtk_widget_show (fixed1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), fixed1, TRUE, TRUE, 0);
  gtk_widget_set_size_request (fixed1, 400, 200);

  label1 = gtk_label_new ("<b>Are you sure you want to Exit?"
      "\nCurrent Form will NOT be saved!!!"
      "\nIf you want to save current form and exit,"
      "\nclick 'Save and Exit' button</b>");
  gtk_widget_show (label1);
  gtk_fixed_put (GTK_FIXED (fixed1), label1, 40, 64);
  gtk_widget_set_size_request (label1, 300, 130);
  gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);

  image1 = gtk_image_new_from_stock ("gtk-dialog-question", GTK_ICON_SIZE_DIALOG);
  gtk_widget_show (image1);
  gtk_fixed_put (GTK_FIXED (fixed1), image1, 160, 24);
  gtk_widget_set_size_request (image1, 40, 40);

  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancelbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), cancelbutton1, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);

  okbutton1 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), okbutton1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);

  g_signal_connect_swapped ((gpointer) dialog1, "delete_event",
                            G_CALLBACK (gtk_true),
                            GTK_OBJECT (dialog1));
  g_signal_connect_swapped ((gpointer) cancelbutton1, "clicked",
                            G_CALLBACK (on_cancelbutton1_clicked),
                            GTK_OBJECT (cancelbutton1));
  g_signal_connect_swapped ((gpointer) okbutton1, "clicked",
                            G_CALLBACK (on_okbutton1_clicked),
                            GTK_OBJECT (okbutton1));

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog1, "dialog1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT (dialog1, fixed1, "fixed1");
  GLADE_HOOKUP_OBJECT (dialog1, label1, "label1");
  GLADE_HOOKUP_OBJECT (dialog1, image1, "image1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog1, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (dialog1, okbutton1, "okbutton1");

  return dialog1;
}


GtkWidget* online_transfer_window (void)
{
  GtkWidget *window1;
  GtkWidget *fixed1;
  GtkWidget *hbuttonbox2;
  GtkWidget *button1;
  GtkWidget *button2;
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *image1;

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window1), "Online transfer");
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);

  fixed1 = gtk_fixed_new ();
  gtk_widget_show (fixed1);
  gtk_container_add (GTK_CONTAINER (window1), fixed1);
  gtk_widget_set_size_request (fixed1, 600, 480);
  gtk_container_set_border_width (GTK_CONTAINER (fixed1), 2);

  hbuttonbox2 = gtk_hbutton_box_new ();
  gtk_widget_show (hbuttonbox2);
  gtk_fixed_put (GTK_FIXED (fixed1), hbuttonbox2, 143, 430);
  gtk_widget_set_size_request (hbuttonbox2, 300, 30);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox2), GTK_BUTTONBOX_SPREAD);

  button1 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (button1);
  gtk_container_add (GTK_CONTAINER (hbuttonbox2), button1);
  GTK_WIDGET_SET_FLAGS (button1, GTK_CAN_DEFAULT);

  button2 = gtk_button_new_from_stock ("gtk-ok");
//  gtk_widget_show (button1);
  gtk_container_add (GTK_CONTAINER (hbuttonbox2), button2);
  GTK_WIDGET_SET_FLAGS (button2, GTK_CAN_DEFAULT);
  /* make the button by default grabbed */
//  GTK_WIDGET_SET_FLAGS(button1, GTK_CAN_DEFAULT);
  gtk_widget_grab_default(button2);
  /* ******************** */


  label1 = gtk_label_new ("Connection Status\n");
  gtk_widget_show (label1);
  gtk_fixed_put (GTK_FIXED (fixed1), label1, 2, 100);
  gtk_widget_set_size_request (label1, 558, 270);
  gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);

  label2 = gtk_label_new ("");
  gtk_widget_show (label2);
  gtk_fixed_put (GTK_FIXED (fixed1), label2, 2, 380);
  gtk_widget_set_size_request (label2, 558, 40);
  gtk_label_set_use_markup (GTK_LABEL (label2), TRUE);

  image1 = create_pixmap (window1, "tt.png");
  gtk_widget_show (image1);
  gtk_fixed_put (GTK_FIXED (fixed1), image1, 208, 8);
  gtk_widget_set_size_request (image1, 142, 70);

  g_signal_connect_swapped ((gpointer) window1, "delete_event",
                            G_CALLBACK (gtk_true),
                            GTK_OBJECT (window1));

  g_signal_connect_swapped ((gpointer) button1, "clicked",
                            G_CALLBACK (on_online_cancel_clicked),
                            GTK_OBJECT (button1));

  g_signal_connect_swapped ((gpointer) button2, "clicked",
                            G_CALLBACK (on_online_ok_clicked),
                            GTK_OBJECT (button2));

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (window1, window1, "window1");
  GLADE_HOOKUP_OBJECT (window1, fixed1, "fixed1");
  GLADE_HOOKUP_OBJECT (window1, hbuttonbox2, "hbuttonbox2");
  GLADE_HOOKUP_OBJECT (window1, button1, "button1");
  GLADE_HOOKUP_OBJECT (window1, button2, "button2");
  GLADE_HOOKUP_OBJECT (window1, label1, "label1");
  GLADE_HOOKUP_OBJECT (window1, label2, "label2");
  GLADE_HOOKUP_OBJECT (window1, image1, "image1");

  return window1;
}

void on_online_cancel_clicked(GtkButton *button, gpointer data)
{
  GtkWidget *ptr;

  submit_cancelled = 1;

  ptr = lookup_widget(GTK_WIDGET(button), "window1");
  if (ptr && GTK_WIDGET_VISIBLE(ptr))
    gtk_widget_hide(ptr);
}

void on_online_ok_clicked(GtkButton *button, gpointer data)
{
  GtkWidget *ptr;

  ptr = lookup_widget(GTK_WIDGET(button), "button2");
  if (ptr && GTK_WIDGET_VISIBLE(ptr))
    gtk_widget_hide(ptr);

  ptr = lookup_widget(GTK_WIDGET(button), "button1");
  if (ptr && !GTK_WIDGET_VISIBLE(ptr))
    gtk_widget_show(ptr);

  ptr = lookup_widget(GTK_WIDGET(button), "window1");
  if (ptr && GTK_WIDGET_VISIBLE(ptr))
    gtk_widget_hide(ptr);

}

void handle_signal(int signo)
{
  PRINTF("handle_signal : %d\n", signo);
  gtk_main_quit();
  exit(signo);
}


/* add photo to the container */
void
add_photo(GtkWidget *container)
{
  if (!load_photo) {
      load_photo = gtk_file_chooser_dialog_new("Load Photo",
	  NULL, /* no parent window */
	  GTK_FILE_CHOOSER_ACTION_OPEN,
	  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
	  NULL);
      gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(load_photo), ROOTDIR);
      g_signal_connect(G_OBJECT(load_photo), "delete_event",
	  G_CALLBACK(gtk_widget_hide_on_delete), NULL);
  }

  if (!GTK_WIDGET_VISIBLE(load_photo))
    gtk_widget_show(load_photo);

  if (gtk_dialog_run(GTK_DIALOG(load_photo)) == GTK_RESPONSE_ACCEPT) {
    if (GTK_WIDGET_VISIBLE(load_photo))
      gtk_widget_hide(load_photo);

    gchar *filename;
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(load_photo));

    if (strcasecmp(&filename[strlen(filename)-3], "jpg")) { /* not a jpeg file */
      show_message("Must have to be a *.jpg file");
      return; /* don't go further */
    }



#ifdef _WIN32
    convert_win_to_posix(filename);
#endif

    GtkWidget *image = gtk_image_new_from_file(filename);

    if (image) {
      gtk_widget_show(image);
      /* attach the filename to the container */
      g_object_set_data_full(G_OBJECT(container), "photo", filename, (GDestroyNotify) g_free);
//      g_free(filename);

      GList *children = gtk_container_get_children(GTK_CONTAINER(container));
      while(children) {
	if (GTK_WIDGET_TYPE(children->data) == GTK_TYPE_IMAGE) {
	  gtk_container_remove(GTK_CONTAINER(container), children->data); /* remove previous pic */
	  /* after gtk_container_remove(), the image is destroyed
	   * automatically
	   */
//	  gtk_widget_destroy(GTK_WIDGET(children->data));
	}
	children = children->next; /* iterate */
      }

      gtk_container_add(GTK_CONTAINER(container), image); /* now add the newly created image */
    }

  }
  else { /* not accepted */
    if(GTK_WIDGET_VISIBLE(load_photo))
      gtk_widget_hide(load_photo);
  }
}


#ifdef _WIN32
void copy_photo(const char *src, const char *dest)
{
  FILE *fp1, *fp2;
  int ch;

  if (!(src && dest))
    return;

  /* if source and destinations are same, don't take any action */
  if (!strcmp(src, dest))
    return;

  fp1 = fopen(src, "rb");
  if (!fp1) {
    perror("fopen: src");
    return;
  }

  fp2 = fopen(dest, "wb");
  if (!fp2) {
    perror("fopen: dest");
    goto error;
  }

  while ((ch = getc(fp1)) != EOF) {
    if (putc(ch, fp2) == EOF) {
      fprintf(stderr, "putc");
      break;
    }
  }

error:
  fclose(fp1);
  fclose(fp2);
}

#else
void copy_photo(const char *src, const char *dest)
{
  int fd1, fd2;
  char buffer[80]; /* read buffer */
  int ret;

  if (!(src && dest))
    return;

  /* if source and destinations are same, don't take any action */
  if (!strcmp(src, dest))
    return;

  fd1 = open(src, O_RDONLY);
  if (fd1 < 0)
    return;

  fd2 = creat(dest, 0666);
  if (fd2 < 0)
    goto error;

  while ((ret = read(fd1, buffer, sizeof(buffer))) > 0) {
    if (write(fd2, buffer, ret) <= 0) {
      perror("write");
      break;
    }
  }

error:
  close(fd1);
  close(fd2);
}
#endif /* !_WIN32 */

#ifdef _WIN32
/* convert win32 path to posix path */
char *convert_win_to_posix(char *path)
{
  char *temp = path;

  while (*temp) {
    if (*temp == '\\')
      *temp = '/';
    temp++;
  }

  return path;
}
#endif /* !_WIN32 */


GtkWidget*
create_options_window (void)
{
  if (options_window) {
    if (!GTK_WIDGET_VISIBLE(options_window))
      gtk_widget_show(options_window);
    return options_window;
  }

  GtkWidget *window1;
  GtkWidget *vbox1;
  GtkWidget *frame1;
  GtkWidget *alignment1;
  GtkWidget *table1;
  GtkWidget *label2;
  GtkWidget *label3;
  GtkWidget *label4;
  GtkWidget *checkbutton1;
  GtkWidget *checkbutton2;
  GtkWidget *checkbutton3;
  GtkWidget *label6;
  GtkWidget *label7;
  GtkWidget *label8;
  GtkWidget *label10;
  GtkWidget *label9;
  GtkWidget *checkbutton4;
  GtkWidget *checkbutton5;
  GtkWidget *checkbutton6;
  GtkWidget *checkbutton7;
  GtkWidget *checkbutton8;
  GtkWidget *label1;
  GtkWidget *hbuttonbox1;
  GtkWidget *button1;

  options_window = window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window1), "Enable/Disable Entries");
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
//  gtk_window_set_skip_taskbar_hint (GTK_WINDOW (window1), TRUE);
  gtk_widget_show(options_window); /* display by default */

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (window1), vbox1);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (vbox1), frame1, TRUE, TRUE, 0);

  alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (frame1), alignment1);

  table1 = gtk_table_new (8, 2, TRUE);
  gtk_widget_show (table1);
  gtk_container_add (GTK_CONTAINER (alignment1), table1);
  gtk_container_set_border_width (GTK_CONTAINER (table1), 2);
  gtk_table_set_row_spacings (GTK_TABLE (table1), 2);
  gtk_table_set_col_spacings (GTK_TABLE (table1), 2);

  label2 = gtk_label_new ("<span foreground=\"maroon\">Father's Name</span>");
  gtk_widget_show (label2);
  gtk_table_attach (GTK_TABLE (table1), label2, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label2), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label2), 0, 0.5);

  label3 = gtk_label_new ("<span foreground=\"maroon\">Mother's Name</span>");
  gtk_widget_show (label3);
  gtk_table_attach (GTK_TABLE (table1), label3, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label3), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label3), 0, 0.5);

  label4 = gtk_label_new ("<span foreground=\"maroon\">Date of Birth</span>");
  gtk_widget_show (label4);
  gtk_table_attach (GTK_TABLE (table1), label4, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label4), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label4), 0, 0.5);

  checkbutton1 = gtk_check_button_new_with_mnemonic ("Check to ignore");
  gtk_widget_show (checkbutton1);
  gtk_table_attach (GTK_TABLE (table1), checkbutton1, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton2 = gtk_check_button_new_with_mnemonic ("Check to ignore");
  gtk_widget_show (checkbutton2);
  gtk_table_attach (GTK_TABLE (table1), checkbutton2, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton3 = gtk_check_button_new_with_mnemonic ("Check to ignore");
  gtk_widget_show (checkbutton3);
  gtk_table_attach (GTK_TABLE (table1), checkbutton3, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label6 = gtk_label_new ("<span foreground=\"maroon\">Previous Year</span>");
  gtk_widget_show (label6);
  gtk_table_attach (GTK_TABLE (table1), label6, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label6), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label6), 0, 0.5);

  label7 = gtk_label_new ("<span foreground=\"maroon\">Previous Roll</span>");
  gtk_widget_show (label7);
  gtk_table_attach (GTK_TABLE (table1), label7, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label7), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label7), 0, 0.5);

  label8 = gtk_label_new ("<span foreground=\"maroon\">Previous Registration</span>");
  gtk_widget_show (label8);
  gtk_table_attach (GTK_TABLE (table1), label8, 0, 1, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label8), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label8), 0, 0.5);

  label10 = gtk_label_new ("<span foreground=\"maroon\">Previous Session</span>");
  gtk_widget_show (label10);
  gtk_table_attach (GTK_TABLE (table1), label10, 0, 1, 6, 7,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label10), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label10), 0, 0.5);

  label9 = gtk_label_new ("<span foreground=\"maroon\">Photo</span>");
  gtk_widget_show (label9);
  gtk_table_attach (GTK_TABLE (table1), label9, 0, 1, 7, 8,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label9), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label9), 0, 0.5);

  checkbutton4 = gtk_check_button_new_with_mnemonic ("Check to ignore (H. S. C Only)");
  gtk_widget_show (checkbutton4);
  gtk_table_attach (GTK_TABLE (table1), checkbutton4, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton5 = gtk_check_button_new_with_mnemonic ("Check to ignore (H. S. C Only)");
  gtk_widget_show (checkbutton5);
  gtk_table_attach (GTK_TABLE (table1), checkbutton5, 1, 2, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton6 = gtk_check_button_new_with_mnemonic ("Check to ignore (H. S. C Only)");
  gtk_widget_show (checkbutton6);
  gtk_table_attach (GTK_TABLE (table1), checkbutton6, 1, 2, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton7 = gtk_check_button_new_with_mnemonic ("Check to ignore (H. S. C Only)");
  gtk_widget_show (checkbutton7);
  gtk_table_attach (GTK_TABLE (table1), checkbutton7, 1, 2, 6, 7,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton8 = gtk_check_button_new_with_mnemonic ("Check to enable");
  gtk_widget_show (checkbutton8);
  gtk_table_attach (GTK_TABLE (table1), checkbutton8, 1, 2, 7, 8,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label1 = gtk_label_new ("<b>Ignore Entries</b>");
  gtk_widget_show (label1);
  gtk_frame_set_label_widget (GTK_FRAME (frame1), label1);
  gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);

  hbuttonbox1 = gtk_hbutton_box_new ();
  gtk_widget_show (hbuttonbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbuttonbox1), 5);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_SPREAD);

  button1 = gtk_button_new_from_stock ("gtk-close");
  gtk_widget_show (button1);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button1);
  GTK_WIDGET_SET_FLAGS (button1, GTK_CAN_DEFAULT);

  g_signal_connect_swapped ((gpointer) window1, "delete_event",
                            G_CALLBACK (gtk_widget_hide),
                            GTK_OBJECT (window1));
  g_signal_connect_swapped ((gpointer) checkbutton1, "toggled",
                            G_CALLBACK (on_fa_name_toggled),
                            GTK_OBJECT (checkbutton1));
  g_signal_connect_swapped ((gpointer) checkbutton2, "toggled",
                            G_CALLBACK (on_mo_name_toggled),
                            GTK_OBJECT (checkbutton2));
  g_signal_connect_swapped ((gpointer) checkbutton3, "toggled",
                            G_CALLBACK (on_birthdate_toggled),
                            GTK_OBJECT (checkbutton3));
  g_signal_connect_swapped ((gpointer) checkbutton4, "toggled",
                            G_CALLBACK (on_prev_year_toggled),
                            GTK_OBJECT (checkbutton4));
  g_signal_connect_swapped ((gpointer) checkbutton5, "toggled",
                            G_CALLBACK (on_prev_roll_toggled),
                            GTK_OBJECT (checkbutton5));
  g_signal_connect_swapped ((gpointer) checkbutton6, "toggled",
                            G_CALLBACK (on_prev_reg_toggled),
                            GTK_OBJECT (checkbutton6));
  g_signal_connect_swapped ((gpointer) checkbutton7, "toggled",
                            G_CALLBACK (on_prev_sess_toggled),
                            GTK_OBJECT (checkbutton7));
  g_signal_connect_swapped ((gpointer) checkbutton8, "toggled",
                            G_CALLBACK (on_photo_toggled),
                            GTK_OBJECT (checkbutton8));
  g_signal_connect_swapped ((gpointer) button1, "clicked",
                            G_CALLBACK (on_ignore_close_clicked),
                            GTK_OBJECT (button1));

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (window1, window1, "window1");
  GLADE_HOOKUP_OBJECT (window1, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (window1, frame1, "frame1");
  GLADE_HOOKUP_OBJECT (window1, alignment1, "alignment1");
  GLADE_HOOKUP_OBJECT (window1, table1, "table1");
  GLADE_HOOKUP_OBJECT (window1, label2, "label2");
  GLADE_HOOKUP_OBJECT (window1, label3, "label3");
  GLADE_HOOKUP_OBJECT (window1, label4, "label4");
  GLADE_HOOKUP_OBJECT (window1, checkbutton1, "checkbutton1");
  GLADE_HOOKUP_OBJECT (window1, checkbutton2, "checkbutton2");
  GLADE_HOOKUP_OBJECT (window1, checkbutton3, "checkbutton3");
  GLADE_HOOKUP_OBJECT (window1, label6, "label6");
  GLADE_HOOKUP_OBJECT (window1, label7, "label7");
  GLADE_HOOKUP_OBJECT (window1, label8, "label8");
  GLADE_HOOKUP_OBJECT (window1, label10, "label10");
  GLADE_HOOKUP_OBJECT (window1, label9, "label9");
  GLADE_HOOKUP_OBJECT (window1, checkbutton4, "checkbutton4");
  GLADE_HOOKUP_OBJECT (window1, checkbutton5, "checkbutton5");
  GLADE_HOOKUP_OBJECT (window1, checkbutton6, "checkbutton6");
  GLADE_HOOKUP_OBJECT (window1, checkbutton7, "checkbutton7");
  GLADE_HOOKUP_OBJECT (window1, checkbutton8, "checkbutton8");
  GLADE_HOOKUP_OBJECT (window1, label1, "label1");
  GLADE_HOOKUP_OBJECT (window1, hbuttonbox1, "hbuttonbox1");
  GLADE_HOOKUP_OBJECT (window1, button1, "button1");

  return window1;
}

void init_ignored(void)
{
  ignore_fa_name = 0;
  ignore_mo_name = 0;
  ignore_birthdate = 0;
  ignore_totlist = 0;
  ignore_prev_year = 0;
  ignore_prev_roll = 0;
  ignore_prev_reg = 0;
  ignore_prev_sess = 0;
  ignore_photo = 1; /* ignore photo by default */
}

/*
 * FILEPATTERN => %s_%04d_%s_%04d_%s.txt
 * */
char *construct_filename(char *buf, const char *gender, int roll,
    const char *section, int totlist, const char *group)
{
  char gen[2]; /* gender (M/F) */
  char s[2]; /* section */
  char grp[2];

  /* M_0002_A_0001_sc.txt  => 20 (minimum) */
  /* M_0001_A_0001_home_eco.txt  => 26 (maximum) */

  if (!strcmp(group, "Science"))
    strcpy(grp, "0");
  else if (!strcmp(group, "Social Science"))
    strcpy(grp, "1");
  else if (!strcmp(group, "Arts"))
    strcpy(grp, "2");
  else if (!strcmp(group, "Commerce"))
    strcpy(grp, "3");
  else if (!strcmp(group, "Agriculture"))
    strcpy(grp, "4");
  else if (!strcmp(group, "home_eco"))
    strcpy(grp, "5");
  else if (!strcmp(group, "Islamic Studies"))
    strcpy(grp, "6");
  else if (!strcmp(group, "Music"))
    strcpy(grp, "7");
  else if (!strcmp(group, "Business"))
    strcpy(grp, "8");
  else
    strcpy(grp, "u");

  if (!strcmp(gender, "Male"))
    strcpy(gen, "M");
  else
    strcpy(gen, "F");

  if (!strcmp(section, " "))
    strcpy(s, "_");
  else
    strcpy(s, section);

  sprintf(buf, FILEPATTERN, gen, roll, s, totlist, grp);

  return buf;
}
//  sprintf(filename, FILEPATTERN, section, gender, classroll);

#ifdef _WIN32
int send_photo(const char *filename)
{
  FILE *fp;
  int ch;
//  int i;
  size_t size;
  char *basename;
  int nwrite;
  int err;
  int ret;

  static char rbuf[20];

  fp = fopen(filename, "rb");
  if (!fp) {
    perror("fopen: filename");
    return 1;
  }

  fseek(fp, 0L, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  basename = strrchr(filename, '/');
  basename += 1; /* skip the '/' */

  sprintf(error_msg, "BINARY %d %s\n", size, basename);
//  printf(error_msg);
  nwrite = strlen(error_msg);

  PRINTF("Checking whether image transfer enabled ...\n");
  if ((ret = writen(sockfd, error_msg, nwrite)) != nwrite) {
    PRINTF("[FAILED]\n");
    show_message("send_photo: writen: [FAILED]");
    err = 1;
    goto error;
  }

  if ((ret = readline(sockfd, rbuf, sizeof(rbuf))) < 4) {
    PRINTF("send_photo: error: readline\n");
    show_message("send_photo: readline: [FAILED]");
    err = 1;
    goto error;
  }

  err = (int) strtol(rbuf, (char **) NULL, 10);
  switch (err) {
    case E_IMG_NOSUPPORT:
      show_message("Image transfer not allowed!");
      err = 1;
      break;
    case 500:
      err = 0;
      break;
    default:
      sprintf(error_msg, "Message From Server : %s", rbuf);
      show_message(error_msg);
      err = 1;
      break;
  }

  if (err == 1)
    goto error;

  PRINTF("Sending Photo\n");

  nwrite = 0;
  while (1) {
    ch = getc(fp);

    if (ch == EOF) {
      if ((ret = writen(sockfd, error_msg, nwrite)) != nwrite) {
	PRINTF("[FAILED]\n");
	show_message("Image transfer FAILED!");
	err = 1;
	goto error;
      }
      err = 0;
      break;
    }
    else if (nwrite >= sizeof(error_msg)) {
      if ((ret = writen(sockfd, error_msg, nwrite)) != nwrite) {
	PRINTF("[FAILED]\n");
	show_message("Image transfer FAILED!");
	err = 1;
	goto error;
      }
      nwrite = 0; /* reset the buffer read index */
    }
    error_msg[nwrite++] = ch;
  }

  if ((ret = readline(sockfd, rbuf, sizeof(rbuf))) < 4) {
    PRINTF("send_photo: error: readline\n");
    show_message("send_photo: readline: [FAILED]");
    err = 1;
    goto error;
  }

  err = (int) strtol(rbuf, (char **) NULL, 10);
  switch (err) {
    case 500:
      err = 0;
      PRINTF("Image transfer successful\n");
//      show_message("Image transfer successful!");
      break;
    default:
      sprintf(error_msg, "Message From Server : %s", rbuf);
      show_message(error_msg);
      err = 1;
      break;
  }

error:
  fclose(fp);

  return err;

}

#else
int send_photo(const char *filename)
{
  int fd;
  int ret;
  int err;
  int size;
  char *basename;
  int nwrite;

  static char rbuf[20];

  /* if source and destinations are same, don't take any action */
  fd = open(filename, O_RDONLY);
  if (fd < 0)
    return 1;

  size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);

  basename = strrchr(filename, '/');
  basename += 1; /* skip the '/' */

  sprintf(error_msg, "BINARY %d %s\n", size, basename);
//  printf(error_msg);
  nwrite = strlen(error_msg);

  PRINTF("Checking whether image transfer enabled ...\n");
  if ((ret = writen(sockfd, error_msg, nwrite)) != nwrite) {
    PRINTF("[FAILED]\n");
    show_message("send_photo: writen: [FAILED]");
    err = 1;
    goto error;
  }

  if ((ret = readline(sockfd, rbuf, sizeof(rbuf))) < 4) {
    PRINTF("send_photo: error: readline\n");
    show_message("send_photo: readline: [FAILED]");
    err = 1;
    goto error;
  }

  err = (int) strtol(rbuf, (char **) NULL, 10);
  switch (err) {
    case E_IMG_NOSUPPORT:
      show_message("Image transfer not allowed!");
      err = 1;
      break;
    case 500:
      err = 0;
      break;
    default:
      sprintf(error_msg, "Message From Server : %s", rbuf);
      show_message(error_msg);
      err = 1;
      break;
  }

  if (err == 1)
    goto error;

  while ((ret = read(fd, error_msg, sizeof(error_msg))) > 0) {
    if (write(sockfd, error_msg, ret) <= 0) {
      perror("write");
      err = 1;
      break;
    }
  }

  if ((ret = readline(sockfd, rbuf, sizeof(rbuf))) < 4) {
    PRINTF("send_photo: error: readline\n");
    show_message("send_photo: readline: [FAILED]");
    err = 1;
    goto error;
  }

  err = (int) strtol(rbuf, (char **) NULL, 10);
  switch (err) {
    case 500:
      err = 0;
      PRINTF("Image transfer successful\n");
//      show_message("Image transfer successful!");
      break;
    default:
      sprintf(error_msg, "Message From Server : %s", rbuf);
      show_message(error_msg);
      err = 1;
      break;
  }

error:
  close(fd);

  return err;
}
#endif /* !_WIN32 */
