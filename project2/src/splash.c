#include "includes.h"
#include "copyright_png.h"

void create_splash(int delay) {
  GtkWidget *splash;
  GdkPixbuf *copyright;
  splash = gtk_window_new(GTK_WINDOW_POPUP);
  gtk_window_set_position(GTK_WINDOW(splash), GTK_WIN_POS_CENTER);
//  gtk_window_maximize(GTK_WINDOW(splash));
  gtk_window_set_decorated(GTK_WINDOW(splash), FALSE);


  copyright = gdk_pixbuf_new_from_inline(-1, copyright_png, FALSE, NULL);
  GtkWidget *logo = gtk_image_new_from_pixbuf(copyright);

  gtk_container_add(GTK_CONTAINER(splash), logo);

  gtk_widget_show_all(splash);
  while(gtk_events_pending())
    gtk_main_iteration();

  SLEEP(delay);
  if (copyright)
    gdk_pixbuf_unref(copyright);
  gtk_object_destroy(GTK_OBJECT(splash));
}
