/*
 * get-theme-dir.c
 */
#include <gtk/gtk.h>

#define SLEEP(sec) sleep(sec*1000)
#define MKDIR(dir,mode) mkdir(dir)

int main(int argc, char **argv)
{
  char *dirname;
  char **files;

  printf("%04d\n", 110);

  /* load custom theme */
  gtk_rc_add_default_file("/usr/share/themes/Bluecurve/gtk-2.0/gtkrc");
  gtk_init(&argc, &argv);

  dirname = gtk_rc_get_theme_dir();
  printf("Theme directory : %s\n", dirname);


  files = gtk_rc_get_default_files();

  int i;

  for (i=0; files[i]; i++) {
    printf("%s\n", files[i]);
  }

  g_free(dirname);

  gchar *mdir;
  mdir = gtk_rc_get_module_dir();
  printf("MODULE DIR: %s\n", mdir);
  g_free(mdir);

  gchar *tdir;
  tdir = gtk_rc_get_theme_dir();
  printf("THEME DIR : %s\n", tdir);
  g_free(tdir);

  GtkWidget *win;
  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width(GTK_CONTAINER(win), 2);
  g_signal_connect(G_OBJECT(win), "delete_event",
      G_CALLBACK(gtk_true), NULL);
  g_signal_connect(G_OBJECT(win), "destroy_event",
      G_CALLBACK(gtk_object_destroy), NULL);

  GtkWidget *button;

  button = gtk_button_new_from_stock("gtk-close");
  gtk_container_add(GTK_CONTAINER(win), button);
  g_signal_connect(G_OBJECT(button), "clicked",
      G_CALLBACK(gtk_main_quit), NULL);


  gtk_widget_show_all(win);
  gtk_main();

  return 0;
}
