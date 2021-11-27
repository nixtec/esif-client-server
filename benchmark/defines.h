/*
 * defines.h
 * definitions shared by all
 * all MUST strictly follow the rules defined in this file
 */
#ifndef _DEFINES_H_
#define _DEFINES_H_

#define ARRAY_LEN(a) (sizeof(a)/sizeof(a[0]))
#define BUFLEN 80              /* static character buffer length */
#define GROUPS 9               /* number of groups available */

#define SUBCODEFILE "subcode.txt" /* subject code list file */
#define COLLEGEFILE "college.txt"  /* college code name file */
#define PAIRFILE "pair.txt"       /* subject code pair matching file */
#define SPECIALFILE "special.txt" /* special codes */

#define ACCEPTCHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ.- "
#define ACCEPTNUMBERS "0123456789"


#ifdef _WIN32
#define ROOTDIR "C:"  /* needed for edition initialization */
#define MKDIR(dir,mode) mkdir(dir)
#define SLEEP(sec) Sleep((sec)*1000)
/* LOGFILE will be used for windows as we're not using terminal */
#define LOGFILE "c:/log.txt"  /* for windows (CD) */
#define ERRFILE "c:/err.txt"  /* for windows (CD) */
#else
#define ROOTDIR "/home/ayub"  /* needed for edition initialization */
#define MKDIR(dir,mode) mkdir(dir, mode)
#define SLEEP(sec) sleep(sec)
#define LOGFILE "log.txt"     /* for testing */
#define ERRFILE "err.txt"     /* for testing */
#endif

/* handle situation if the log file can't be opened for writing */
extern int logok;   /* defined in main.c */
#define PRINTF if (logok) printf




#define MAXDIRNAME  200      /* maximum directory name length */
#define MAXFILENAME 20       /* maximum filename length */

/* information file header string pattern */
/* use sprintf() to build the pattern string */
/* (char *):(char *):(int %0.4d):(boolean) */
/* section, gender, (int) class_roll, sent, student's name */
#define HEADPATTERN "%s:%s:%04d:%d:%s\n\n"
/* (char *)_(char *)_(int %0.4d).txt */
/* section, gender, (int) class_roll */
#define FILEPATTERN "%s_%s_%04d.txt"

#define NUM_TOTAL_SUB 17   /* number of subject entry widget */
#define WITH_F_MIN 12 /* minimum number of codes to be filled in with 4th sub */
#define WITHOUT_F_MIN 10 /* minimum number of codes to be filled in without 4th sub */

#define BOARD "Rajshahi"          /* needed for editor initialization */

/* mkdir doesn't have any mode argument in mingw/windows
 * sleep assumes argument as miliseconds in windows
 */

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full(G_OBJECT(component), name, \
      gtk_widget_ref(widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data(G_OBJECT(component), name, widget)

#define DIGEST "sha1"
#define MAGIC "UiPatyYZz49882454iIopzZ"   /* will be sha1 computed */
#define MAXLINE 120     /* maximum length of a line */
#define SERV_PORT 7860

/* encrypted magic buffer */
extern unsigned char magic[]; /* defined in main.c */

#endif     /* _DEFINES_H_ */
