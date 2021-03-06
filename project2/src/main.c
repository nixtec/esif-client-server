/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "includes.h"

#include <gtk/gtk.h>

#include "interface.h"
#include "support.h"

extern int is_ssc; /* defined in myfuncs.c */

unsigned char magic[SHA_DIGEST_LENGTH];


int logok; /* default is 1 (success) */
int sess_inc;

void init_magic(const char *str, unsigned char *md_value);
void create_splash(int delay);



int
main (int argc, char *argv[])
{
  GtkWidget *window1;

#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  logok = 1;
  sess_inc = 2; /* default value */

#ifdef _WIN32
  if (!freopen(LOGFILE, "w", stdout))
    logok = 0; /* failure */
  else
    logok = 1;
  if (logok)
    freopen(ERRFILE, "w", stderr);
  FreeConsole(); /* leave the console */
#endif
  /* load custom theme */
  int i;

  /* options recognized:
   * --theme        => load theme
   * --sessinc=N    => examination year will be N years greater than entered session
   */
  for (i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--theme"))
      gtk_rc_add_default_file("theme/gtk-2.0/gtkrc");
    else if (!strncmp(argv[i], "--sessinc=", 10))
      sscanf(argv[i], "--sessinc=%d", &sess_inc);
  }

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  logok = 0; /* disable logging */

//  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");
  add_pixmap_directory ("pixmaps");
  add_pixmap_directory ("../pixmaps");
  add_pixmap_directory ("./");
  add_pixmap_directory (""); /* so that all absolute paths are successful */

  window1 = create_window1 ();
  while (gtk_events_pending())
    gtk_main_iteration();

  create_splash(5);

  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  gtk_widget_show (window1);

  is_ssc = 1; /* S. S. C is selected by default */

  memset(magic, 0, sizeof(magic));
  init_magic(MAGIC, magic);

  gtk_main ();
  return 0;
}


void init_magic(const char *str, unsigned char *md_value)
{
  SHA_CTX shactx;

  SHA1_Init(&shactx);
  SHA1_Update(&shactx, str, strlen(str));
  SHA1_Final(md_value, &shactx);
}
