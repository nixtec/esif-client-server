/*
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#include "includes.h"
#include "myfuncs.h"


GtkWidget*
create_window1 (void)
{
  GtkWidget *window1;
  GdkPixbuf *window1_icon_pixbuf;
  GtkWidget *vbox1;
  GtkWidget *image1;
//  GtkWidget *label1;
  GtkWidget *upperimg; /* upper image */
  GtkWidget *lowerimg; /* lower image */
  GtkWidget *vbox2;
  GtkWidget *hbox1;
  GtkWidget *radiobutton1;
  GSList *radiobutton1_group = NULL;
  GtkWidget *hbox2;
  GtkWidget *radiobutton2;
  GtkWidget *hbox3;
  GtkWidget *button1;
  GtkWidget *alignment2;
  GtkWidget *hbox8;
  GtkWidget *image4;
  GtkWidget *label54;
//  GtkWidget *label3;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (window1, "window1");
  gtk_container_set_border_width (GTK_CONTAINER (window1), 2);
  gtk_window_set_title (GTK_WINDOW (window1), _("Online Registration System"));
  gtk_window_set_position (GTK_WINDOW (window1), GTK_WIN_POS_CENTER);
  gtk_window_maximize(GTK_WINDOW(window1));
  window1_icon_pixbuf = create_pixbuf ("bug-buddy.png");
  if (window1_icon_pixbuf)
    {
      gtk_window_set_icon (GTK_WINDOW (window1), window1_icon_pixbuf);
      gdk_pixbuf_unref (window1_icon_pixbuf);
    }

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox1, "vbox1");
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (window1), vbox1);
  gtk_container_set_border_width (GTK_CONTAINER (vbox1), 1);

  image1 = create_pixmap (window1, "tt.png");
  gtk_widget_set_name (image1, "image1");
  gtk_widget_show (image1);
  gtk_box_pack_start (GTK_BOX (vbox1), image1, FALSE, FALSE, 2);

  upperimg = create_pixmap (window1, "upperpic.png");
  gtk_widget_set_name (image1, "upperimg");
  gtk_widget_show (upperimg);
  gtk_box_pack_start (GTK_BOX (vbox1), upperimg, FALSE, FALSE, 2);

  /*
  label1 = gtk_label_new (_("<span foreground=\"#872222\" size=\"xx-large\">\n  Board of Intermediate and Secondary Education, Rajshahi  \n\n                 Student's Online Registration System  \n</span>\n"));
  gtk_widget_set_name (label1, "label1");
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (vbox1), label1, FALSE, FALSE, 0);
  gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_LEFT);
  */

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox2, "vbox2");
  gtk_widget_show (vbox2);
  gtk_box_pack_start (GTK_BOX (vbox1), vbox2, FALSE, FALSE, 0);

  hbox1 = gtk_hbox_new (TRUE, 0);
  gtk_widget_set_name (hbox1, "hbox1");
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox1, FALSE, FALSE, 0);

  radiobutton1 = gtk_radio_button_new_with_mnemonic (NULL, _("S. S. C"));
  gtk_widget_set_name (radiobutton1, "radiobutton1");
  gtk_widget_show (radiobutton1);
  gtk_box_pack_start (GTK_BOX (hbox1), radiobutton1, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (radiobutton1), 1);
  gtk_tooltips_set_tip (tooltips, radiobutton1, _("S. S. C Student's Information Form"), NULL);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton1), radiobutton1_group);
  radiobutton1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton1));


  hbox2 = gtk_hbox_new (TRUE, 0);
  gtk_widget_set_name (hbox2, "hbox2");
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox2, TRUE, TRUE, 0);

  radiobutton2 = gtk_radio_button_new_with_mnemonic (NULL, _("H. S. C"));
  gtk_widget_set_name (radiobutton2, "radiobutton2");
  gtk_widget_show (radiobutton2);
  gtk_box_pack_start (GTK_BOX (hbox2), radiobutton2, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (radiobutton2), 1);
  gtk_tooltips_set_tip (tooltips, radiobutton2, _("H. S. C Student's Information Form"), NULL);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton2), radiobutton1_group);
  radiobutton1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton2));

  /* disable the H. S. C radio button */
  /*
  gtk_widget_set_sensitive(radiobutton2, FALSE);
  */

  hbox3 = gtk_hbox_new (TRUE, 0);
  gtk_widget_set_name (hbox3, "hbox3");
  gtk_widget_show (hbox3);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox3, FALSE, FALSE, 0);

  button1 = gtk_button_new ();
  gtk_widget_set_name (button1, "button1");
  gtk_widget_show (button1);
  gtk_box_pack_start (GTK_BOX (hbox3), button1, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (button1), 2);
  gtk_tooltips_set_tip (tooltips, button1, _("Go Inside"), NULL);

  /* make the button by default grabbed */
  GTK_WIDGET_SET_FLAGS(button1, GTK_CAN_DEFAULT);
  gtk_widget_grab_default(button1);
  /* ******************** */

  alignment2 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_set_name (alignment2, "alignment2");
  gtk_widget_show (alignment2);
  gtk_container_add (GTK_CONTAINER (button1), alignment2);

  hbox8 = gtk_hbox_new (FALSE, 2);
  gtk_widget_set_name (hbox8, "hbox8");
  gtk_widget_show (hbox8);
  gtk_container_add (GTK_CONTAINER (alignment2), hbox8);

  image4 = create_pixmap (window1, "goto-24.png");
  gtk_widget_set_name (image4, "image4");
  gtk_widget_show (image4);
  gtk_box_pack_start (GTK_BOX (hbox8), image4, FALSE, FALSE, 0);

  label54 = gtk_label_new_with_mnemonic (_("Go"));
  gtk_widget_set_name (label54, "label54");
  gtk_widget_show (label54);
  gtk_box_pack_start (GTK_BOX (hbox8), label54, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label54), GTK_JUSTIFY_LEFT);

  lowerimg = create_pixmap (window1, "lowerpic.png");
  gtk_widget_set_name (image1, "lowerimg");
  gtk_widget_show (lowerimg);
  gtk_box_pack_start (GTK_BOX (vbox1), lowerimg, FALSE, FALSE, 2);

  /*
  label3 = gtk_label_new (_("<span foreground=\"#2F2E78\" size=\"xx-large\">\n<i>                                     Planning by\n                             Prof. Md. Nazrul Islam\n                                      Chairman\nBoard of Intermediate and Secondary Education, Rajshahi</i>\n</span>\n"));
  gtk_widget_set_name (label3, "label3");
  gtk_widget_show (label3);
  gtk_box_pack_start (GTK_BOX (vbox1), label3, FALSE, FALSE, 0);
  gtk_label_set_use_markup (GTK_LABEL (label3), TRUE);
  gtk_label_set_justify (GTK_LABEL (label3), GTK_JUSTIFY_LEFT);
  */

  g_signal_connect_swapped ((gpointer) window1, "delete_event",
                            G_CALLBACK (on_window1_delete_event),
                            GTK_OBJECT (window1));
  g_signal_connect_swapped ((gpointer) button1, "clicked",
                            G_CALLBACK (on_button1_clicked),
                            GTK_OBJECT (button1));
  g_signal_connect_swapped ((gpointer) radiobutton1, "toggled",
                            G_CALLBACK (on_radiobutton1_toggled),
                            GTK_OBJECT (radiobutton1));
  g_signal_connect_swapped ((gpointer) radiobutton2, "toggled",
                            G_CALLBACK (on_radiobutton2_toggled),
                            GTK_OBJECT (radiobutton2));

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (window1, window1, "window1");
  GLADE_HOOKUP_OBJECT (window1, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (window1, image1, "image1");
  GLADE_HOOKUP_OBJECT (window1, upperimg, "upperimg");
//  GLADE_HOOKUP_OBJECT (window1, label1, "label1");
  GLADE_HOOKUP_OBJECT (window1, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (window1, hbox1, "hbox1");
  GLADE_HOOKUP_OBJECT (window1, radiobutton1, "radiobutton1");
  GLADE_HOOKUP_OBJECT (window1, hbox2, "hbox2");
  GLADE_HOOKUP_OBJECT (window1, radiobutton2, "radiobutton2");
  GLADE_HOOKUP_OBJECT (window1, hbox3, "hbox3");
  GLADE_HOOKUP_OBJECT (window1, button1, "button1");
  GLADE_HOOKUP_OBJECT (window1, alignment2, "alignment2");
  GLADE_HOOKUP_OBJECT (window1, hbox8, "hbox8");
  GLADE_HOOKUP_OBJECT (window1, image4, "image4");
  GLADE_HOOKUP_OBJECT (window1, label54, "label54");
  GLADE_HOOKUP_OBJECT (window1, lowerimg, "lowerimg");
//  GLADE_HOOKUP_OBJECT (window1, label3, "label3");
  GLADE_HOOKUP_OBJECT_NO_REF (window1, tooltips, "tooltips");

  return window1;
}

GtkWidget*
create_window2 (void)
{
  GtkWidget *window2;
  GdkPixbuf *window2_icon_pixbuf;
  GtkWidget *vbox3;
  GtkWidget *hbox4;
  GtkWidget *image2;
  GtkWidget *label4;
  GtkWidget *scrolledwindow1;
  GtkWidget *viewport1;
  GtkWidget *frame1;
  GtkWidget *fixed1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;
  GtkWidget *frame8;
  GtkWidget *combo6;
  GList *combo6_items = NULL;
  GtkWidget *combo_entry6;
  GtkWidget *label18;
  GtkWidget *table1;
  GtkWidget *label19;
  GtkWidget *label20;
  GtkWidget *entry9;
  GtkWidget *entry8;
  GtkWidget *frame3;
  GtkWidget *entry5;
  GtkWidget *label12;
  GtkWidget *frame5;
  GtkWidget *entry6;
  GtkWidget *label14;
  GtkWidget *frame4;
  GtkWidget *combo2;
  GList *combo2_items = NULL;
  GtkWidget *combo_entry2;
  GtkWidget *label13;
  GtkWidget *frame7;
  GtkWidget *entry7;
  GtkWidget *label17;
  GtkWidget *frame9;
  GtkWidget *fixed2;
  GtkWidget *frame11;
  GtkWidget *entry10;
  GtkWidget *label23;
  GtkWidget *frame12;
  GtkWidget *entry11;
  GtkWidget *label24;
  GtkWidget *frame13;
  GtkWidget *entry12;
  GtkWidget *label25;
  GtkWidget *frame14;
  GtkWidget *hbox7;
  GtkWidget *entry13;
  GtkWidget *label53;
  GtkWidget *entry14;
  GtkWidget *label26;
  GtkWidget *frame10;
  GtkWidget *combo7;
  GList *combo7_items = NULL;
  GtkWidget *combo_entry7;
  GtkWidget *label22;
  GtkWidget *label21;
  GtkWidget *frame16;
  GtkWidget *combo9;
  GList *combo9_items = NULL;
  GtkWidget *combo_entry9;
  GtkWidget *label28;
  GtkWidget *frame17;
  GtkWidget *table2;
  GtkWidget *frame25;
  GtkWidget *entry15;
  GtkWidget *label37;
  GtkWidget *frame26;
  GtkWidget *entry16;
  GtkWidget *label38;
  GtkWidget *frame27;
  GtkWidget *entry17;
  GtkWidget *label39;
  GtkWidget *frame28;
  GtkWidget *entry18;
  GtkWidget *label40;
  GtkWidget *frame29;
  GtkWidget *entry19;
  GtkWidget *label41;
  GtkWidget *frame30;
  GtkWidget *entry20;
  GtkWidget *label42;
  GtkWidget *frame31;
  GtkWidget *entry21;
  GtkWidget *label43;
  GtkWidget *frame32;
  GtkWidget *entry22;
  GtkWidget *label44;
  GtkWidget *frame33;
  GtkWidget *entry23;
  GtkWidget *label45;
  GtkWidget *frame34;
  GtkWidget *entry24;
  GtkWidget *label46;
  GtkWidget *frame35;
  GtkWidget *entry25;
  GtkWidget *label47;
  GtkWidget *frame36;
  GtkWidget *entry26;
  GtkWidget *label48;
  GtkWidget *frame37;
  GtkWidget *entry27;
  GtkWidget *label49;
  GtkWidget *frame38;
  GtkWidget *entry28;
  GtkWidget *label50;
  /* extra subject code entry start */
  GtkWidget *frame_ex_1;
  GtkWidget *entry29;
  GtkWidget *label_ex_1;
  GtkWidget *frame_ex_2;
  GtkWidget *entry30;
  GtkWidget *label_ex_2;
  /* extra subject code entry end */
  GtkWidget *frame39;
  GtkWidget *entry31; /* fourth subject */
  GtkWidget *label51;
  GtkWidget *frame40;
  GtkWidget *button5;
  GtkWidget *label52;
  GtkWidget *label29;
  GtkWidget *label8;
  GtkWidget *label9;
  GtkWidget *label10;
  GtkWidget *frame2;
  GtkWidget *combo1;
  GList *combo1_items = NULL;
  GtkWidget *combo_entry1;
  GtkWidget *label11;
  GtkWidget *frame6;
  GtkWidget *hbox5;
  GtkWidget *combo3;
  GList *combo3_items = NULL;
  GtkWidget *combo_entry3;
  GtkWidget *combo4;
  GList *combo4_items = NULL;
  GtkWidget *combo_entry4;
#ifdef ____NO____
  GtkWidget *combo5;
  GList *combo5_items = NULL;
#endif
  GtkWidget *combo_entry5;
  GtkWidget *label15;
  GtkWidget *frame15;
  GtkWidget *combo8;
  GList *combo8_items = NULL;
  GtkWidget *combo_entry8;
  GtkWidget *label27;
  GtkWidget *label7;
  GtkWidget *hbuttonbox1;
  GtkWidget *button2;
  GtkWidget *button3;
  GtkWidget *button4;
  /*
  GtkWidget *alignment1;
  GtkWidget *hbox6;
  GtkWidget *image3;
  GtkWidget *label16;
  */
  GtkTooltips *tooltips;


  int idx = 0;
  GtkWidget **subcodes = NULL;

#ifdef ____NO____
  time_t curtime;
  char year[6];
  char year0[6]; /* year string */
  char year1[6]; /* year string */
  char year2[6]; /* year string */
  char year3[6]; /* year string */
  char year4[6]; /* year string */
  char year5[6]; /* year string */
  char year6[6]; /* year string */
  char year7[6]; /* year string */
  char year8[6]; /* year string */
  char year9[6]; /* year string */
  int yearval;
#endif

  /* allocate pointer */
  subcodes = (GtkWidget **) malloc((NUM_TOTAL_SUB+1) * sizeof(GtkWidget *));

#ifdef ____NO____
  curtime = time(NULL);
  strftime(year, 5, "%Y", localtime(&curtime)); /* current year */
  yearval = atoi(year);
  yearval -= 15;
  /* **************** */
#endif

  tooltips = gtk_tooltips_new ();

  window2 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (window2, "window2");
  gtk_container_set_border_width (GTK_CONTAINER (window2), 2);
  gtk_window_set_title (GTK_WINDOW (window2), _("Student's Information Form (SIF)"));
  gtk_window_set_position (GTK_WINDOW (window2), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size (GTK_WINDOW (window2), 800, 560);
  gtk_window_maximize(GTK_WINDOW(window2));
  window2_icon_pixbuf = create_pixbuf ("bug-buddy.png");


  if (window2_icon_pixbuf)
    {
      gtk_window_set_icon (GTK_WINDOW (window2), window2_icon_pixbuf);
      gdk_pixbuf_unref (window2_icon_pixbuf);
    }

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox3, "vbox3");
  gtk_widget_show (vbox3);
  gtk_container_add (GTK_CONTAINER (window2), vbox3);

  hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_widget_set_name (hbox4, "hbox4");
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (vbox3), hbox4, FALSE, TRUE, 0);

  image2 = create_pixmap (window2, "tt-right.png");
  gtk_widget_set_name (image2, "image2");
  gtk_widget_show (image2);
  gtk_box_pack_start (GTK_BOX (hbox4), image2, FALSE, FALSE, 0);


  /* modified by ayub */

  char label4_str[256];
  int s = (int) strtol(all_session, (char **) NULL, 10);
  s += sess_inc; /* if session is 1998-99, examination is 2000 */

  sprintf(label4_str, "<span foreground=\"#872222\">\n                S. S. C / H. S. C Examination, %04d\n        Student's Information Form (SIF) for Registration\nBoard of Intermediate and Secondary Education, Rajshahi\n</span>", s);
  label4 = gtk_label_new (_(label4_str));

  /* **************** */


  /*
  label4 = gtk_label_new (_("<span foreground=\"#872222\">\n                      H. S. C Examination, 2005\n      Student's Information Form (SIF) for Registration\nBoard of Intermediate and Secondary Education, Rajshahi\n</span>"));
  */
  gtk_widget_set_name (label4, "label4");
  gtk_widget_show (label4);
  gtk_box_pack_start (GTK_BOX (hbox4), label4, TRUE, FALSE, 0);
  gtk_label_set_use_markup (GTK_LABEL (label4), TRUE);
  gtk_label_set_justify (GTK_LABEL (label4), GTK_JUSTIFY_LEFT);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_set_name (scrolledwindow1, "scrolledwindow1");
  gtk_widget_show (scrolledwindow1);
  gtk_box_pack_start (GTK_BOX (vbox3), scrolledwindow1, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_SHADOW_ETCHED_IN);

  viewport1 = gtk_viewport_new (NULL, NULL);
  gtk_widget_set_name (viewport1, "viewport1");
  gtk_widget_show (viewport1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), viewport1);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame1, "frame1");
  gtk_widget_show (frame1);
  gtk_container_add (GTK_CONTAINER (viewport1), frame1);

  fixed1 = gtk_fixed_new ();
  gtk_widget_set_name (fixed1, "fixed1");
  gtk_widget_show (fixed1);
  gtk_container_add (GTK_CONTAINER (frame1), fixed1);

  entry2 = gtk_entry_new ();
  gtk_widget_set_name (entry2, "entry2");
  gtk_widget_show (entry2);
  gtk_fixed_put (GTK_FIXED (fixed1), entry2, 168, 8);
  gtk_widget_set_size_request (entry2, 450, 20);
  gtk_tooltips_set_tip (tooltips, entry2, _("Student's Name (maximum 40 characters)"), NULL);
  gtk_entry_set_max_length (GTK_ENTRY (entry2), 40);

  entry3 = gtk_entry_new ();
  gtk_widget_set_name (entry3, "entry3");
  gtk_widget_show (entry3);
  gtk_fixed_put (GTK_FIXED (fixed1), entry3, 168, 32);
  gtk_widget_set_size_request (entry3, 450, 20);
  gtk_tooltips_set_tip (tooltips, entry3, _("Father's Name (maximum 40 characters)"), NULL);
  gtk_entry_set_max_length (GTK_ENTRY (entry3), 40);

  entry4 = gtk_entry_new ();
  gtk_widget_set_name (entry4, "entry4");
  gtk_widget_show (entry4);
  gtk_fixed_put (GTK_FIXED (fixed1), entry4, 168, 56);
  gtk_widget_set_size_request (entry4, 450, 20);
  gtk_tooltips_set_tip (tooltips, entry4, _("Mother's Name (maximum 40 characters)"), NULL);
  gtk_entry_set_max_length (GTK_ENTRY (entry4), 40);

  /* add photo widgets here */

  GtkWidget *photo_frame = gtk_frame_new("Photo");
  gtk_widget_set_name(photo_frame, "photo_frame");
  gtk_widget_show(photo_frame);
  gtk_fixed_put(GTK_FIXED(fixed1), photo_frame, 630, 0);
  gtk_widget_set_size_request(photo_frame, 90, 90);
  gtk_tooltips_set_tip (tooltips, photo_frame, _("Photo must have to be 70x70 pixel"), NULL);
  GLADE_HOOKUP_OBJECT (window2, photo_frame, "photo_frame");

  GtkWidget *photo_hint = create_pixmap(window2, "photo-hint.png");
  if (photo_hint) {
    gtk_widget_show(photo_hint);
    gtk_container_add(GTK_CONTAINER(photo_frame), photo_hint);
  }
  /* ********************** */

  frame8 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame8, "frame8");
  gtk_widget_show (frame8);
  gtk_fixed_put (GTK_FIXED (fixed1), frame8, 8, 152);
  gtk_widget_set_size_request (frame8, 70, 52);

  combo6 = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo6)->popwin),
                     "GladeParentKey", combo6);
  gtk_widget_set_name (combo6, "combo6");
  gtk_widget_show (combo6);
  gtk_container_add (GTK_CONTAINER (frame8), combo6);
  combo6_items = g_list_append (combo6_items, (gpointer) _(" "));
  combo6_items = g_list_append (combo6_items, (gpointer) _("A"));
  combo6_items = g_list_append (combo6_items, (gpointer) _("B"));
  combo6_items = g_list_append (combo6_items, (gpointer) _("C"));
  combo6_items = g_list_append (combo6_items, (gpointer) _("D"));
  combo6_items = g_list_append (combo6_items, (gpointer) _("E"));
  combo6_items = g_list_append (combo6_items, (gpointer) _("F"));
  gtk_combo_set_popdown_strings (GTK_COMBO (combo6), combo6_items);
  g_list_free (combo6_items);

  combo_entry6 = GTK_COMBO (combo6)->entry;
  gtk_widget_set_name (combo_entry6, "combo_entry6");
  gtk_widget_show (combo_entry6);
  gtk_tooltips_set_tip (tooltips, combo_entry6, _("Your Class Section"), NULL);
  gtk_entry_set_max_length (GTK_ENTRY (combo_entry6), 1);
  gtk_entry_set_alignment(GTK_ENTRY(combo_entry6), 0.5);
  gtk_editable_set_editable (GTK_EDITABLE (combo_entry6), FALSE);

  label18 = gtk_label_new (_("10. <span foreground=\"#191F7A\">Section</span>"));
  gtk_widget_set_name (label18, "label18");
  gtk_widget_show (label18);
  gtk_frame_set_label_widget (GTK_FRAME (frame8), label18);
  gtk_label_set_use_markup (GTK_LABEL (label18), TRUE);
  gtk_label_set_justify (GTK_LABEL (label18), GTK_JUSTIFY_LEFT);

  table1 = gtk_table_new (2, 2, FALSE);
  gtk_widget_set_name (table1, "table1");
  gtk_widget_show (table1);
  gtk_fixed_put (GTK_FIXED (fixed1), table1, 90, 158);
  gtk_widget_set_size_request (table1, 600, 40);

  label19 = gtk_label_new (_("11.     <span foreground=\"#191F7A\">Institution Code</span>"));
  gtk_widget_set_name (label19, "label19");
  gtk_widget_show (label19);
  gtk_table_attach (GTK_TABLE (table1), label19, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label19), TRUE);
  gtk_label_set_justify (GTK_LABEL (label19), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label19), 0, 0.5);

  label20 = gtk_label_new (_("                                         <span foreground=\"#191F7A\">Institution Name</span>"));
  gtk_widget_set_name (label20, "label20");
  gtk_widget_show (label20);
  gtk_table_attach (GTK_TABLE (table1), label20, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label20), TRUE);
  gtk_label_set_justify (GTK_LABEL (label20), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label20), 0, 0.5);

  entry9 = gtk_entry_new ();
  gtk_widget_set_name (entry9, "entry9");
  gtk_widget_show (entry9);
  gtk_table_attach (GTK_TABLE (table1), entry9, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, entry9, _("Institution Name (all caps, maximum 70 characters)"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry9), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry9), 70);

  entry8 = gtk_entry_new ();
  gtk_widget_set_name (entry8, "entry8");
  gtk_widget_show (entry8);
  gtk_table_attach (GTK_TABLE (table1), entry8, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, entry8, _("Institution Code (4 digits)"), NULL);
//  gtk_widget_set_size_request (entry8, 120, 25);
  gtk_entry_set_alignment(GTK_ENTRY(entry8), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry8), 4);


  frame3 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame3, "frame3");
  gtk_widget_show (frame3);
  gtk_fixed_put (GTK_FIXED (fixed1), frame3, 136, 96);
  gtk_widget_set_size_request (frame3, 95, 45);

  GtkWidget *zzz_fixed1 = gtk_fixed_new ();
  gtk_widget_show (zzz_fixed1);
  gtk_container_add (GTK_CONTAINER (frame3), zzz_fixed1);

  entry5 = gtk_entry_new ();
#ifdef ____NO____
  gtk_entry_set_text(GTK_ENTRY(entry5), year); /* fill session */
#endif
  gtk_entry_set_text(GTK_ENTRY(entry5), all_session); /* fill session */
  gtk_editable_set_editable(GTK_EDITABLE(entry5), FALSE); /* disable session to be modified */
  gtk_widget_set_name (entry5, "entry5");
  gtk_widget_show (entry5);
  gtk_fixed_put(GTK_FIXED(zzz_fixed1), entry5, 2, 0);
  gtk_widget_set_size_request(entry5, 40, 24);
//  gtk_container_add (GTK_CONTAINER (frame3), entry5);
  gtk_tooltips_set_tip (tooltips, entry5, _("Current Education Session (4 digits)"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry5), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry5), 4);


  /* dummy entry */
  int zzz = (int) strtol(all_session, (char **) NULL, 10);
  zzz += 1; /* next year (session) */
  char zzz_str[5];
  sprintf(zzz_str, "%04d", zzz);
  GtkWidget *dummy_entry5 = gtk_entry_new();

  gtk_entry_set_text(GTK_ENTRY(dummy_entry5), zzz_str); /* fill session */
  gtk_editable_set_editable(GTK_EDITABLE(dummy_entry5), FALSE); /* disable session to be modified */
  gtk_widget_set_name (dummy_entry5, "dummy_entry5");
  gtk_widget_show (dummy_entry5);
  gtk_fixed_put(GTK_FIXED(zzz_fixed1), dummy_entry5, 47, 0);
  gtk_widget_set_size_request(dummy_entry5, 40, 24);
//  gtk_container_add (GTK_CONTAINER (frame3), entry5);
  gtk_tooltips_set_tip (tooltips, dummy_entry5, _("Current Education Session (4 digits)"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(dummy_entry5), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (dummy_entry5), 4);

  /* zzzzzzzzz */

  label12 = gtk_label_new (_("5. <span foreground=\"#191F7A\">Session</span>"));
  gtk_widget_set_name (label12, "label12");
  gtk_widget_show (label12);
  gtk_frame_set_label_widget (GTK_FRAME (frame3), label12);
  gtk_label_set_use_markup (GTK_LABEL (label12), TRUE);
  gtk_label_set_justify (GTK_LABEL (label12), GTK_JUSTIFY_LEFT);

  frame5 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame5, "frame5");
  gtk_widget_show (frame5);
  gtk_fixed_put (GTK_FIXED (fixed1), frame5, 340, 96);
  gtk_widget_set_size_request (frame5, 90, 45);

  entry6 = gtk_entry_new ();
  gtk_widget_set_name (entry6, "entry6");
  gtk_widget_show (entry6);
  gtk_container_add (GTK_CONTAINER (frame5), entry6);
  gtk_tooltips_set_tip (tooltips, entry6, _("Class Roll Number (4 digits)"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry6), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry6), 4);

  label14 = gtk_label_new (_("7. <span foreground=\"#191F7A\">Class Roll</span>"));
  gtk_widget_set_name (label14, "label14");
  gtk_widget_show (label14);
  gtk_frame_set_label_widget (GTK_FRAME (frame5), label14);
  gtk_label_set_use_markup (GTK_LABEL (label14), TRUE);
  gtk_label_set_justify (GTK_LABEL (label14), GTK_JUSTIFY_LEFT);

  frame4 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame4, "frame4");
  gtk_widget_show (frame4);
  gtk_fixed_put (GTK_FIXED (fixed1), frame4, 240, 96);
  gtk_widget_set_size_request (frame4, 90, 45);

  combo2 = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo2)->popwin),
                     "GladeParentKey", combo2);
  gtk_widget_set_name (combo2, "combo2");
  gtk_widget_show (combo2);
  gtk_container_add (GTK_CONTAINER (frame4), combo2);
  combo2_items = g_list_append (combo2_items, (gpointer) _("Dhaka"));
  gtk_combo_set_popdown_strings (GTK_COMBO (combo2), combo2_items);
  g_list_free (combo2_items);

  combo_entry2 = GTK_COMBO (combo2)->entry;
  gtk_widget_set_name (combo_entry2, "combo_entry2");
  gtk_widget_show (combo_entry2);
  gtk_tooltips_set_tip (tooltips, combo_entry2, _("Your Board of Exam"), NULL);
  gtk_editable_set_editable (GTK_EDITABLE (combo_entry2), FALSE);

  label13 = gtk_label_new (_("6. <span foreground=\"#191F7A\">Board</span>"));
  gtk_widget_set_name (label13, "label13");
  gtk_widget_show (label13);
  gtk_frame_set_label_widget (GTK_FRAME (frame4), label13);
  gtk_label_set_use_markup (GTK_LABEL (label13), TRUE);
  gtk_label_set_justify (GTK_LABEL (label13), GTK_JUSTIFY_LEFT);

  frame7 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame7, "frame7");
  gtk_widget_show (frame7);
  gtk_fixed_put (GTK_FIXED (fixed1), frame7, 628, 88);
  gtk_widget_set_size_request (frame7, 90, 60);

  entry7 = gtk_entry_new ();
  gtk_widget_set_name (entry7, "entry7");
  gtk_widget_show (entry7);
  gtk_container_add (GTK_CONTAINER (frame7), entry7);
  gtk_tooltips_set_tip (tooltips, entry7, _("TOT list serial Number (4 digits)"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry7), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry7), 4);

  label17 = gtk_label_new (_("9. <span foreground=\"#191F7A\">TOT List\n    Serial No</span>"));
  gtk_widget_set_name (label17, "label17");
  gtk_widget_show (label17);
  gtk_frame_set_label_widget (GTK_FRAME (frame7), label17);
  gtk_label_set_use_markup (GTK_LABEL (label17), TRUE);
  gtk_label_set_justify (GTK_LABEL (label17), GTK_JUSTIFY_LEFT);

  frame9 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame9, "frame9");
  gtk_widget_show (frame9);
  gtk_fixed_put (GTK_FIXED (fixed1), frame9, 8, 210);
  gtk_widget_set_size_request (frame9, 478, 70);

  fixed2 = gtk_fixed_new ();
  gtk_widget_set_name (fixed2, "fixed2");
  gtk_widget_show (fixed2);
  gtk_container_add (GTK_CONTAINER (frame9), fixed2);

  frame11 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame11, "frame11");
  gtk_widget_show (frame11);
  gtk_fixed_put (GTK_FIXED (fixed2), frame11, 104, 8);
  gtk_widget_set_size_request (frame11, 59, 40);

  entry10 = gtk_entry_new ();
  gtk_widget_set_name (entry10, "entry10");
  gtk_widget_show (entry10);
  gtk_container_add (GTK_CONTAINER (frame11), entry10);
  gtk_tooltips_set_tip (tooltips, entry10, _("Year of passing (4 digits)"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry10), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry10), 4);

  label23 = gtk_label_new (_("Year"));
  gtk_widget_set_name (label23, "label23");
  gtk_widget_show (label23);
  gtk_frame_set_label_widget (GTK_FRAME (frame11), label23);
  gtk_label_set_justify (GTK_LABEL (label23), GTK_JUSTIFY_LEFT);

  frame12 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame12, "frame12");
  gtk_widget_show (frame12);
  gtk_fixed_put (GTK_FIXED (fixed2), frame12, 170, 8);
  gtk_widget_set_size_request (frame12, 80, 42);

  entry11 = gtk_entry_new ();
  gtk_widget_set_name (entry11, "entry11");
  gtk_widget_show (entry11);
  gtk_container_add (GTK_CONTAINER (frame12), entry11);
  gtk_tooltips_set_tip (tooltips, entry11, _("S. S. C or equivalent Roll Number (6 digits)"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry11), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry11), 6);

  label24 = gtk_label_new (_("Roll Number"));
  gtk_widget_set_name (label24, "label24");
  gtk_widget_show (label24);
  gtk_frame_set_label_widget (GTK_FRAME (frame12), label24);
  gtk_label_set_justify (GTK_LABEL (label24), GTK_JUSTIFY_LEFT);

  frame13 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame13, "frame13");
  gtk_widget_show (frame13);
  gtk_fixed_put (GTK_FIXED (fixed2), frame13, 260, 8);
  gtk_widget_set_size_request (frame13, 80, 42);

  entry12 = gtk_entry_new ();
  gtk_widget_set_name (entry12, "entry12");
  gtk_widget_show (entry12);
  gtk_container_add (GTK_CONTAINER (frame13), entry12);
  gtk_tooltips_set_tip (tooltips, entry12, _("S. S. C or equivalent Registration Number (6 digits)"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry12), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry12), 6);

  label25 = gtk_label_new (_("Registration"));
  gtk_widget_set_name (label25, "label25");
  gtk_widget_show (label25);
  gtk_frame_set_label_widget (GTK_FRAME (frame13), label25);
  gtk_label_set_justify (GTK_LABEL (label25), GTK_JUSTIFY_LEFT);

  frame14 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame14, "frame14");
  gtk_widget_show (frame14);
  gtk_fixed_put (GTK_FIXED (fixed2), frame14, 350, 8);
  gtk_widget_set_size_request (frame14, 120, 42);

  hbox7 = gtk_hbox_new (FALSE, 0);
  gtk_widget_set_name (hbox7, "hbox7");
  gtk_widget_show (hbox7);
  gtk_container_add (GTK_CONTAINER (frame14), hbox7);

  entry13 = gtk_entry_new ();
  gtk_widget_set_name (entry13, "entry13");
  gtk_widget_show (entry13);
  gtk_box_pack_start (GTK_BOX (hbox7), entry13, TRUE, TRUE, 0);
  gtk_tooltips_set_tip (tooltips, entry13, _("4 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry13), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry13), 4);

  label53 = gtk_label_new (_("-"));
  gtk_widget_set_name (label53, "label53");
  gtk_widget_show (label53);
  gtk_box_pack_start (GTK_BOX (hbox7), label53, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label53), GTK_JUSTIFY_LEFT);

  entry14 = gtk_entry_new ();
  gtk_widget_set_name (entry14, "entry14");
  gtk_widget_show (entry14);
  gtk_box_pack_start (GTK_BOX (hbox7), entry14, TRUE, TRUE, 0);
  gtk_tooltips_set_tip (tooltips, entry14, _("4 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry14), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry14), 4);

  label26 = gtk_label_new (_("Session"));
  gtk_widget_set_name (label26, "label26");
  gtk_widget_show (label26);
  gtk_frame_set_label_widget (GTK_FRAME (frame14), label26);
  gtk_label_set_justify (GTK_LABEL (label26), GTK_JUSTIFY_LEFT);

  frame10 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame10, "frame10");
  gtk_widget_show (frame10);
  gtk_fixed_put (GTK_FIXED (fixed2), frame10, 0, 8);
  gtk_widget_set_size_request (frame10, 95, 40);

  combo7 = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo7)->popwin),
                     "GladeParentKey", combo7);
  gtk_widget_set_name (combo7, "combo7");
  gtk_widget_show (combo7);
  gtk_container_add (GTK_CONTAINER (frame10), combo7);
  combo7_items = g_list_append (combo7_items, (gpointer) _("Dhaka"));
  combo7_items = g_list_append (combo7_items, (gpointer) _("Rajshahi"));
  combo7_items = g_list_append (combo7_items, (gpointer) _("Comilla"));
  combo7_items = g_list_append (combo7_items, (gpointer) _("Jessore"));
  combo7_items = g_list_append (combo7_items, (gpointer) _("Chittagong"));
  combo7_items = g_list_append (combo7_items, (gpointer) _("Barisal"));
  combo7_items = g_list_append (combo7_items, (gpointer) _("Sylhet"));
  combo7_items = g_list_append (combo7_items, (gpointer) _("Madrasa"));
  combo7_items = g_list_append (combo7_items, (gpointer) _("Technical"));
  combo7_items = g_list_append (combo7_items, (gpointer) _("Others"));
  gtk_combo_set_popdown_strings (GTK_COMBO (combo7), combo7_items);
  g_list_free (combo7_items);

  combo_entry7 = GTK_COMBO (combo7)->entry;
  gtk_widget_set_name (combo_entry7, "combo_entry7");
  gtk_widget_show (combo_entry7);
  gtk_tooltips_set_tip (tooltips, combo_entry7, _("Which board did you complete your previous exam from"), NULL);
  gtk_editable_set_editable (GTK_EDITABLE (combo_entry7), FALSE);

  label22 = gtk_label_new (_("Board"));
  gtk_widget_set_name (label22, "label22");
  gtk_widget_show (label22);
  gtk_frame_set_label_widget (GTK_FRAME (frame10), label22);
  gtk_label_set_justify (GTK_LABEL (label22), GTK_JUSTIFY_LEFT);

  label21 = gtk_label_new (_("12. <span foreground=\"red\">Information of passing S. S. C or equivalent exam</span>"
	" (For H. S. C only)"));
  gtk_widget_set_name (label21, "label21");
  gtk_widget_show (label21);
  gtk_frame_set_label_widget (GTK_FRAME (frame9), label21);
  gtk_label_set_use_markup (GTK_LABEL (label21), TRUE);
  gtk_label_set_justify (GTK_LABEL (label21), GTK_JUSTIFY_LEFT);

  frame16 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame16, "frame16");
  gtk_widget_show (frame16);
  gtk_fixed_put (GTK_FIXED (fixed1), frame16, 600, 210);
  gtk_widget_set_size_request (frame16, 90, 70);

  combo9 = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo9)->popwin),
                     "GladeParentKey", combo9);
  gtk_widget_set_name (combo9, "combo9");
  gtk_widget_show (combo9);
  gtk_container_add (GTK_CONTAINER (frame16), combo9);
  combo9_items = g_list_append (combo9_items, (gpointer) _("Islam"));
  combo9_items = g_list_append (combo9_items, (gpointer) _("Hindu"));
  combo9_items = g_list_append (combo9_items, (gpointer) _("Buddist"));
  combo9_items = g_list_append (combo9_items, (gpointer) _("Christian"));
  combo9_items = g_list_append (combo9_items, (gpointer) _("Others"));
  gtk_combo_set_popdown_strings (GTK_COMBO (combo9), combo9_items);
  g_list_free (combo9_items);

  combo_entry9 = GTK_COMBO (combo9)->entry;
  gtk_widget_set_name (combo_entry9, "combo_entry9");
  gtk_widget_show (combo_entry9);
  gtk_tooltips_set_tip (tooltips, combo_entry9, _("Your Religion (Your subject may depend on this)"), NULL);
  gtk_editable_set_editable (GTK_EDITABLE (combo_entry9), FALSE);

  label28 = gtk_label_new (_("14. <span foreground=\"#191F7A\">Religion</span>"));
  gtk_widget_set_name (label28, "label28");
  gtk_widget_show (label28);
  gtk_frame_set_label_widget (GTK_FRAME (frame16), label28);
  gtk_label_set_use_markup (GTK_LABEL (label28), TRUE);
  gtk_label_set_justify (GTK_LABEL (label28), GTK_JUSTIFY_LEFT);

  frame17 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame17, "frame17");
  gtk_widget_show (frame17);
  gtk_fixed_put (GTK_FIXED (fixed1), frame17, 8, 287);
  gtk_widget_set_size_request (frame17, 680, 110);
  gtk_container_set_border_width (GTK_CONTAINER (frame17), 2);

  table2 = gtk_table_new (2, 8, TRUE);
  gtk_widget_set_name (table2, "table2");
  gtk_widget_show (table2);
  gtk_container_add (GTK_CONTAINER (frame17), table2);
  gtk_table_set_row_spacings (GTK_TABLE (table2), 2);
  gtk_table_set_col_spacings (GTK_TABLE (table2), 2);

  frame25 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame25, "frame25");
  gtk_widget_show (frame25);
  gtk_table_attach (GTK_TABLE (table2), frame25, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

  subcodes[idx++] = entry15 = gtk_entry_new ();
  gtk_widget_set_name (entry15, "entry15");
  gtk_widget_show (entry15);
  gtk_container_add (GTK_CONTAINER (frame25), entry15);
  gtk_tooltips_set_tip (tooltips, entry15, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry15), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry15), 3);

  label37 = gtk_label_new (_("Sub 01"));
  gtk_widget_set_name (label37, "label37");
  gtk_widget_show (label37);
  gtk_frame_set_label_widget (GTK_FRAME (frame25), label37);
  gtk_label_set_justify (GTK_LABEL (label37), GTK_JUSTIFY_LEFT);

  frame26 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame26, "frame26");
  gtk_widget_show (frame26);
  gtk_table_attach (GTK_TABLE (table2), frame26, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry16 = gtk_entry_new ();
  gtk_widget_set_name (entry16, "entry16");
  gtk_widget_show (entry16);
  gtk_container_add (GTK_CONTAINER (frame26), entry16);
  gtk_tooltips_set_tip (tooltips, entry16, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry16), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry16), 3);

  label38 = gtk_label_new (_("Sub 02"));
  gtk_widget_set_name (label38, "label38");
  gtk_widget_show (label38);
  gtk_frame_set_label_widget (GTK_FRAME (frame26), label38);
  gtk_label_set_justify (GTK_LABEL (label38), GTK_JUSTIFY_LEFT);

  frame27 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame27, "frame27");
  gtk_widget_show (frame27);
  gtk_table_attach (GTK_TABLE (table2), frame27, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry17 = gtk_entry_new ();
  gtk_widget_set_name (entry17, "entry17");
  gtk_widget_show (entry17);
  gtk_container_add (GTK_CONTAINER (frame27), entry17);
  gtk_tooltips_set_tip (tooltips, entry17, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry17), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry17), 3);

  label39 = gtk_label_new (_("Sub 03"));
  gtk_widget_set_name (label39, "label39");
  gtk_widget_show (label39);
  gtk_frame_set_label_widget (GTK_FRAME (frame27), label39);
  gtk_label_set_justify (GTK_LABEL (label39), GTK_JUSTIFY_LEFT);

  frame28 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame28, "frame28");
  gtk_widget_show (frame28);
  gtk_table_attach (GTK_TABLE (table2), frame28, 3, 4, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry18 = gtk_entry_new ();
  gtk_widget_set_name (entry18, "entry18");
  gtk_widget_show (entry18);
  gtk_container_add (GTK_CONTAINER (frame28), entry18);
  gtk_tooltips_set_tip (tooltips, entry18, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry18), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry18), 3);

  label40 = gtk_label_new (_("Sub 04"));
  gtk_widget_set_name (label40, "label40");
  gtk_widget_show (label40);
  gtk_frame_set_label_widget (GTK_FRAME (frame28), label40);
  gtk_label_set_justify (GTK_LABEL (label40), GTK_JUSTIFY_LEFT);

  frame29 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame29, "frame29");
  gtk_widget_show (frame29);
  gtk_table_attach (GTK_TABLE (table2), frame29, 4, 5, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry19 = gtk_entry_new ();
  gtk_widget_set_name (entry19, "entry19");
  gtk_widget_show (entry19);
  gtk_container_add (GTK_CONTAINER (frame29), entry19);
  gtk_tooltips_set_tip (tooltips, entry19, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry19), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry19), 3);

  label41 = gtk_label_new (_("Sub 05"));
  gtk_widget_set_name (label41, "label41");
  gtk_widget_show (label41);
  gtk_frame_set_label_widget (GTK_FRAME (frame29), label41);
  gtk_label_set_justify (GTK_LABEL (label41), GTK_JUSTIFY_LEFT);

  frame30 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame30, "frame30");
  gtk_widget_show (frame30);
  gtk_table_attach (GTK_TABLE (table2), frame30, 5, 6, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry20 = gtk_entry_new ();
  gtk_widget_set_name (entry20, "entry20");
  gtk_widget_show (entry20);
  gtk_container_add (GTK_CONTAINER (frame30), entry20);
  gtk_tooltips_set_tip (tooltips, entry20, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry20), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry20), 3);

  label42 = gtk_label_new (_("Sub 06"));
  gtk_widget_set_name (label42, "label42");
  gtk_widget_show (label42);
  gtk_frame_set_label_widget (GTK_FRAME (frame30), label42);
  gtk_label_set_justify (GTK_LABEL (label42), GTK_JUSTIFY_LEFT);

  frame31 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame31, "frame31");
  gtk_widget_show (frame31);
  gtk_table_attach (GTK_TABLE (table2), frame31, 6, 7, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry21 = gtk_entry_new ();
  gtk_widget_set_name (entry21, "entry21");
  gtk_widget_show (entry21);
  gtk_container_add (GTK_CONTAINER (frame31), entry21);
  gtk_tooltips_set_tip (tooltips, entry21, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry21), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry21), 3);

  label43 = gtk_label_new (_("Sub 07"));
  gtk_widget_set_name (label43, "label43");
  gtk_widget_show (label43);
  gtk_frame_set_label_widget (GTK_FRAME (frame31), label43);
  gtk_label_set_justify (GTK_LABEL (label43), GTK_JUSTIFY_LEFT);

  frame32 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame32, "frame32");
  gtk_widget_show (frame32);
  gtk_table_attach (GTK_TABLE (table2), frame32, 7, 8, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry22 = gtk_entry_new ();
  gtk_widget_set_name (entry22, "entry22");
  gtk_widget_show (entry22);
  gtk_container_add (GTK_CONTAINER (frame32), entry22);
  gtk_tooltips_set_tip (tooltips, entry22, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry22), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry22), 3);

  label44 = gtk_label_new (_("Sub 08"));
  gtk_widget_set_name (label44, "label44");
  gtk_widget_show (label44);
  gtk_frame_set_label_widget (GTK_FRAME (frame32), label44);
  gtk_label_set_justify (GTK_LABEL (label44), GTK_JUSTIFY_LEFT);

  frame33 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame33, "frame33");
  gtk_widget_show (frame33);
  gtk_table_attach (GTK_TABLE (table2), frame33, 8, 9, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

  subcodes[idx++] = entry23 = gtk_entry_new ();
  gtk_widget_set_name (entry23, "entry23");
  gtk_widget_show (entry23);
  gtk_container_add (GTK_CONTAINER (frame33), entry23);
  gtk_tooltips_set_tip (tooltips, entry23, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry23), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry23), 3);

  label45 = gtk_label_new (_("Sub 09"));
  gtk_widget_set_name (label45, "label45");
  gtk_widget_show (label45);
  gtk_frame_set_label_widget (GTK_FRAME (frame33), label45);
  gtk_label_set_justify (GTK_LABEL (label45), GTK_JUSTIFY_LEFT);

  frame34 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame34, "frame34");
  gtk_widget_show (frame34);
  gtk_table_attach (GTK_TABLE (table2), frame34, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry24 = gtk_entry_new ();
  gtk_widget_set_name (entry24, "entry24");
  gtk_widget_show (entry24);
  gtk_container_add (GTK_CONTAINER (frame34), entry24);
  gtk_tooltips_set_tip (tooltips, entry24, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry24), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry24), 3);

  label46 = gtk_label_new (_("Sub 10"));
  gtk_widget_set_name (label46, "label46");
  gtk_widget_show (label46);
  gtk_frame_set_label_widget (GTK_FRAME (frame34), label46);
  gtk_label_set_justify (GTK_LABEL (label46), GTK_JUSTIFY_LEFT);

  frame35 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame35, "frame35");
  gtk_widget_show (frame35);
  gtk_table_attach (GTK_TABLE (table2), frame35, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry25 = gtk_entry_new ();
  gtk_widget_set_name (entry25, "entry25");
  gtk_widget_show (entry25);
  gtk_container_add (GTK_CONTAINER (frame35), entry25);
  gtk_tooltips_set_tip (tooltips, entry25, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry25), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry25), 3);

  label47 = gtk_label_new (_("Sub 11"));
  gtk_widget_set_name (label47, "label47");
  gtk_widget_show (label47);
  gtk_frame_set_label_widget (GTK_FRAME (frame35), label47);
  gtk_label_set_justify (GTK_LABEL (label47), GTK_JUSTIFY_LEFT);

  frame36 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame36, "frame36");
  gtk_widget_show (frame36);
  gtk_table_attach (GTK_TABLE (table2), frame36, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry26 = gtk_entry_new ();
  gtk_widget_set_name (entry26, "entry26");
  gtk_widget_show (entry26);
  gtk_container_add (GTK_CONTAINER (frame36), entry26);
  gtk_tooltips_set_tip (tooltips, entry26, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry26), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry26), 3);

  label48 = gtk_label_new (_("Sub 12"));
  gtk_widget_set_name (label48, "label48");
  gtk_widget_show (label48);
  gtk_frame_set_label_widget (GTK_FRAME (frame36), label48);
  gtk_label_set_justify (GTK_LABEL (label48), GTK_JUSTIFY_LEFT);

  frame37 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame37, "frame37");
  gtk_widget_show (frame37);
  gtk_table_attach (GTK_TABLE (table2), frame37, 3, 4, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry27 = gtk_entry_new ();
  gtk_widget_set_name (entry27, "entry27");
  gtk_widget_show (entry27);
  gtk_container_add (GTK_CONTAINER (frame37), entry27);
  gtk_tooltips_set_tip (tooltips, entry27, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry27), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry27), 3);

  label49 = gtk_label_new (_("Sub 13"));
  gtk_widget_set_name (label49, "label49");
  gtk_widget_show (label49);
  gtk_frame_set_label_widget (GTK_FRAME (frame37), label49);
  gtk_label_set_justify (GTK_LABEL (label49), GTK_JUSTIFY_LEFT);

  frame38 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame38, "frame38");
  gtk_widget_show (frame38);
  gtk_table_attach (GTK_TABLE (table2), frame38, 4, 5, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry28 = gtk_entry_new ();
  gtk_widget_set_name (entry28, "entry28");
  gtk_widget_show (entry28);
  gtk_container_add (GTK_CONTAINER (frame38), entry28);
  gtk_tooltips_set_tip (tooltips, entry28, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry28), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry28), 3);

  label50 = gtk_label_new (_("Sub 14"));
  gtk_widget_set_name (label50, "label50");
  gtk_widget_show (label50);
  gtk_frame_set_label_widget (GTK_FRAME (frame38), label50);
  gtk_label_set_justify (GTK_LABEL (label50), GTK_JUSTIFY_LEFT);

  /* extra subject code entry start */
  frame_ex_1 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame_ex_1, "frame_ex_1");
  gtk_widget_show (frame_ex_1);
  gtk_table_attach (GTK_TABLE (table2), frame_ex_1, 5, 6, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry29 = gtk_entry_new ();
  gtk_widget_set_name (entry29, "entry29");
  gtk_widget_show (entry29);
  gtk_container_add (GTK_CONTAINER (frame_ex_1), entry29);
  gtk_tooltips_set_tip (tooltips, entry29, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry29), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry29), 3);

  label_ex_1 = gtk_label_new (_("Sub 15"));
  gtk_widget_set_name (label_ex_1, "label_ex_1");
  gtk_widget_show (label_ex_1);
  gtk_frame_set_label_widget (GTK_FRAME (frame_ex_1), label_ex_1);
  gtk_label_set_justify (GTK_LABEL (label_ex_1), GTK_JUSTIFY_LEFT);

  frame_ex_2 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame_ex_2, "frame_ex_2");
  gtk_widget_show (frame_ex_2);
  gtk_table_attach (GTK_TABLE (table2), frame_ex_2, 6, 7, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry30 = gtk_entry_new ();
  gtk_widget_set_name (entry30, "entry30");
  gtk_widget_show (entry30);
  gtk_container_add (GTK_CONTAINER (frame_ex_2), entry30);
  gtk_tooltips_set_tip (tooltips, entry30, _("3 digits"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry30), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry30), 3);

  label_ex_2 = gtk_label_new (_("Sub 16"));
  gtk_widget_set_name (label_ex_2, "label_ex_2");
  gtk_widget_show (label_ex_2);
  gtk_frame_set_label_widget (GTK_FRAME (frame_ex_2), label_ex_2);
  gtk_label_set_justify (GTK_LABEL (label_ex_2), GTK_JUSTIFY_LEFT);

  /* extra subject code entry end */

  frame39 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame39, "frame39");
  gtk_widget_show (frame39);
  gtk_table_attach (GTK_TABLE (table2), frame39, 7, 8, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  subcodes[idx++] = entry31 = gtk_entry_new ();
  subcodes[NUM_TOTAL_SUB] = NULL;
  gtk_widget_set_name (entry31, "entry31");
  gtk_widget_show (entry31);
  gtk_container_add (GTK_CONTAINER (frame39), entry31);
  gtk_tooltips_set_tip (tooltips, entry31, _("Fourth Subject (First Paper)"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(entry31), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry31), 3);

  label51 = gtk_label_new (_("<span foreground=\"#E08E96\">4th Subject</span>"));
  gtk_widget_set_name (label51, "label51");
  gtk_widget_show (label51);
  gtk_frame_set_label_widget (GTK_FRAME (frame39), label51);
  gtk_label_set_use_markup (GTK_LABEL (label51), TRUE);
  gtk_label_set_justify (GTK_LABEL (label51), GTK_JUSTIFY_LEFT);

  frame40 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame40, "frame40");
  gtk_widget_show (frame40);
  gtk_table_attach (GTK_TABLE (table2), frame40, 8, 9, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  button5 = gtk_button_new_with_mnemonic ("_View");
//  button5 = gtk_button_new_from_stock ("gtk-index");
  gtk_widget_set_name (button5, "button5");
  gtk_widget_show (button5);
  gtk_container_add (GTK_CONTAINER (frame40), button5);
  gtk_tooltips_set_tip (tooltips, button5, _("Click Here to view subject code index"), NULL);

  label52 = gtk_label_new (_("<span foreground=\"#7A7DA6\">Syllabus</span>"));
  gtk_widget_set_name (label52, "label52");
  gtk_widget_show (label52);
  gtk_frame_set_label_widget (GTK_FRAME (frame40), label52);
  gtk_label_set_use_markup (GTK_LABEL (label52), TRUE);
  gtk_label_set_justify (GTK_LABEL (label52), GTK_JUSTIFY_LEFT);

  label29 = gtk_label_new (_("15. <span foreground=\"#4A3A6E\">Subject Codes</span>"));
  gtk_widget_set_name (label29, "label29");
  gtk_widget_show (label29);
  gtk_frame_set_label_widget (GTK_FRAME (frame17), label29);
  gtk_label_set_use_markup (GTK_LABEL (label29), TRUE);
  gtk_label_set_justify (GTK_LABEL (label29), GTK_JUSTIFY_LEFT);

  label8 = gtk_label_new (_("1. <span foreground=\"maroon\">Student's Name</span>"));
  gtk_widget_set_name (label8, "label8");
  gtk_widget_show (label8);
  gtk_fixed_put (GTK_FIXED (fixed1), label8, 16, 8);
  gtk_widget_set_size_request (label8, 120, 20);
  gtk_label_set_use_markup (GTK_LABEL (label8), TRUE);
  gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_LEFT);

  label9 = gtk_label_new (_("2. <span foreground=\"maroon\">Father's Name</span>"));
  gtk_widget_set_name (label9, "label9");
  gtk_widget_show (label9);
  gtk_fixed_put (GTK_FIXED (fixed1), label9, 16, 32);
  gtk_widget_set_size_request (label9, 112, 20);
  gtk_label_set_use_markup (GTK_LABEL (label9), TRUE);
  gtk_label_set_justify (GTK_LABEL (label9), GTK_JUSTIFY_LEFT);

  label10 = gtk_label_new (_("3. <span foreground=\"maroon\">Mother's Name</span>"));
  gtk_widget_set_name (label10, "label10");
  gtk_widget_show (label10);
  gtk_fixed_put (GTK_FIXED (fixed1), label10, 15, 56);
  gtk_widget_set_size_request (label10, 120, 20);
  gtk_label_set_use_markup (GTK_LABEL (label10), TRUE);
  gtk_label_set_justify (GTK_LABEL (label10), GTK_JUSTIFY_LEFT);

  frame2 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame2, "frame2");
  gtk_widget_show (frame2);
  gtk_fixed_put (GTK_FIXED (fixed1), frame2, 8, 96);
  gtk_widget_set_size_request (frame2, 120, 45);

  combo1 = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo1)->popwin),
                     "GladeParentKey", combo1);
  gtk_widget_set_name (combo1, "combo1");
  gtk_widget_show (combo1);
  gtk_container_add (GTK_CONTAINER (frame2), combo1);
  combo1_items = g_list_append (combo1_items, (gpointer) _("Science"));
  combo1_items = g_list_append (combo1_items, (gpointer) _("Social Science"));
  combo1_items = g_list_append (combo1_items, (gpointer) _("Arts"));
  combo1_items = g_list_append (combo1_items, (gpointer) _("Commerce"));
  combo1_items = g_list_append (combo1_items, (gpointer) _("Agriculture"));
  combo1_items = g_list_append (combo1_items, (gpointer) _("Home Economics"));
  combo1_items = g_list_append (combo1_items, (gpointer) _("Islamic Studies"));
  combo1_items = g_list_append (combo1_items, (gpointer) _("Music"));
  combo1_items = g_list_append (combo1_items, (gpointer) _("Business"));
  gtk_combo_set_popdown_strings (GTK_COMBO (combo1), combo1_items);
  g_list_free (combo1_items);

  combo_entry1 = GTK_COMBO (combo1)->entry;
  gtk_widget_set_name (combo_entry1, "combo_entry1");
  gtk_widget_show (combo_entry1);
  gtk_tooltips_set_tip (tooltips, combo_entry1, _("Your Group"), NULL);
  gtk_editable_set_editable (GTK_EDITABLE (combo_entry1), FALSE);
  /* attach data to the entry
   * so that we can get it from callback
   * */
  if (subcodes) {
    PRINTF("interface.c : Attaching data to combo_entry1\n");
    g_object_set_data_full(G_OBJECT(combo_entry1), "subentries", (gpointer) subcodes, free);
  }


  label11 = gtk_label_new (_("4. <span foreground=\"#191F7A\">Group</span>"));
  gtk_widget_set_name (label11, "label11");
  gtk_widget_show (label11);
  gtk_frame_set_label_widget (GTK_FRAME (frame2), label11);
  gtk_label_set_use_markup (GTK_LABEL (label11), TRUE);
  gtk_label_set_justify (GTK_LABEL (label11), GTK_JUSTIFY_LEFT);

  frame6 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame6, "frame6");
  gtk_widget_show (frame6);
  gtk_fixed_put (GTK_FIXED (fixed1), frame6, 440, 96);
  gtk_widget_set_size_request (frame6, 180, 45);

  hbox5 = gtk_hbox_new (TRUE, 0);
  gtk_widget_set_name (hbox5, "hbox5");
  gtk_widget_show (hbox5);
  gtk_container_add (GTK_CONTAINER (frame6), hbox5);

  combo3 = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo3)->popwin),
                     "GladeParentKey", combo3);
  gtk_widget_set_name (combo3, "combo3");
  gtk_widget_show (combo3);
  gtk_box_pack_start (GTK_BOX (hbox5), combo3, TRUE, TRUE, 0);
  combo3_items = g_list_append (combo3_items, (gpointer) _("01"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("02"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("03"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("04"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("05"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("06"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("07"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("08"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("09"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("10"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("11"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("12"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("13"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("14"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("15"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("16"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("17"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("18"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("19"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("20"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("21"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("22"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("23"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("24"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("25"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("26"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("27"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("28"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("29"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("30"));
  combo3_items = g_list_append (combo3_items, (gpointer) _("31"));
  gtk_combo_set_popdown_strings (GTK_COMBO (combo3), combo3_items);
  g_list_free (combo3_items);

  combo_entry3 = GTK_COMBO (combo3)->entry;
  gtk_widget_set_name (combo_entry3, "combo_entry3");
  gtk_widget_show (combo_entry3);
  gtk_tooltips_set_tip (tooltips, combo_entry3, _("Day"), NULL);
  gtk_entry_set_max_length (GTK_ENTRY (combo_entry3), 2);
  gtk_editable_set_editable (GTK_EDITABLE (combo_entry3), FALSE);

  combo4 = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo4)->popwin),
                     "GladeParentKey", combo4);
  gtk_widget_set_name (combo4, "combo4");
  gtk_widget_show (combo4);
  gtk_box_pack_start (GTK_BOX (hbox5), combo4, TRUE, TRUE, 0);
  combo4_items = g_list_append (combo4_items, (gpointer) _("Jan"));
  combo4_items = g_list_append (combo4_items, (gpointer) _("Feb"));
  combo4_items = g_list_append (combo4_items, (gpointer) _("Mar"));
  combo4_items = g_list_append (combo4_items, (gpointer) _("Apr"));
  combo4_items = g_list_append (combo4_items, (gpointer) _("May"));
  combo4_items = g_list_append (combo4_items, (gpointer) _("Jun"));
  combo4_items = g_list_append (combo4_items, (gpointer) _("Jul"));
  combo4_items = g_list_append (combo4_items, (gpointer) _("Aug"));
  combo4_items = g_list_append (combo4_items, (gpointer) _("Sep"));
  combo4_items = g_list_append (combo4_items, (gpointer) _("Oct"));
  combo4_items = g_list_append (combo4_items, (gpointer) _("Nov"));
  combo4_items = g_list_append (combo4_items, (gpointer) _("Dec"));
  gtk_combo_set_popdown_strings (GTK_COMBO (combo4), combo4_items);
  g_list_free (combo4_items);

  combo_entry4 = GTK_COMBO (combo4)->entry;
  gtk_widget_set_name (combo_entry4, "combo_entry4");
  gtk_widget_show (combo_entry4);
  gtk_tooltips_set_tip (tooltips, combo_entry4, _("Month"), NULL);
  gtk_entry_set_max_length (GTK_ENTRY (combo_entry4), 3);
  gtk_editable_set_editable (GTK_EDITABLE (combo_entry4), FALSE);

#ifdef ___NO___
  combo5 = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo5)->popwin),
                     "GladeParentKey", combo5);
  gtk_widget_set_name (combo5, "combo5");
  gtk_widget_show (combo5);
  gtk_box_pack_start (GTK_BOX (hbox5), combo5, TRUE, TRUE, 0);



  /* build year of birth list */
  sprintf(year0, "%d", yearval);
//  PRINTF("year0 = %s\n", year0);
  combo5_items = g_list_append (combo5_items, (gpointer) _(year0));
  yearval--;
  sprintf(year1, "%d", yearval);
//  PRINTF("year1 = %s\n", year1);
  combo5_items = g_list_append (combo5_items, (gpointer) _(year1));
  yearval--;
  sprintf(year2, "%d", yearval);
//  PRINTF("year2 = %s\n", year2);
  combo5_items = g_list_append (combo5_items, (gpointer) _(year2));
  yearval--;
  sprintf(year3, "%d", yearval);
//  PRINTF("year3 = %s\n", year3);
  combo5_items = g_list_append (combo5_items, (gpointer) _(year3));
  yearval--;
  sprintf(year4, "%d", yearval);
//  PRINTF("year4 = %s\n", year4);
  combo5_items = g_list_append (combo5_items, (gpointer) _(year4));
  yearval--;
  sprintf(year5, "%d", yearval);
//  PRINTF("year5 = %s\n", year5);
  combo5_items = g_list_append (combo5_items, (gpointer) _(year5));
  yearval--;
  sprintf(year6, "%d", yearval);
//  PRINTF("year6 = %s\n", year6);
  combo5_items = g_list_append (combo5_items, (gpointer) _(year6));
  yearval--;
  sprintf(year7, "%d", yearval);
//  PRINTF("year7 = %s\n", year7);
  combo5_items = g_list_append (combo5_items, (gpointer) _(year7));
  yearval--;
  sprintf(year8, "%d", yearval);
//  PRINTF("year8 = %s\n", year8);
  combo5_items = g_list_append (combo5_items, (gpointer) _(year8));
  yearval--;
  sprintf(year9, "%d", yearval);
//  PRINTF("year9 = %s\n", year9);
  combo5_items = g_list_append (combo5_items, (gpointer) _(year9));
  yearval--;

  /* ............... */


  /*
  combo5_items = g_list_append (combo5_items, (gpointer) _("1990"));
  combo5_items = g_list_append (combo5_items, (gpointer) _("1989"));
  combo5_items = g_list_append (combo5_items, (gpointer) _("1988"));
  combo5_items = g_list_append (combo5_items, (gpointer) _("1987"));
  combo5_items = g_list_append (combo5_items, (gpointer) _("1986"));
  */

  gtk_combo_set_popdown_strings (GTK_COMBO (combo5), combo5_items);
  g_list_free (combo5_items);
#endif

//  combo_entry5 = GTK_COMBO (combo5)->entry;
  combo_entry5 = gtk_entry_new();
  gtk_box_pack_start (GTK_BOX (hbox5), combo_entry5, TRUE, TRUE, 0);
  gtk_widget_set_name (combo_entry5, "combo_entry5");
  gtk_widget_show (combo_entry5);
  gtk_tooltips_set_tip (tooltips, combo_entry5, _("Year of Birth (4 digits)"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(combo_entry5), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (combo_entry5), 4);

  label15 = gtk_label_new (_("8. <span foreground=\"#191F7A\">Date of Birth</span>"));
  gtk_widget_set_name (label15, "label15");
  gtk_widget_show (label15);
  gtk_frame_set_label_widget (GTK_FRAME (frame6), label15);
  gtk_label_set_use_markup (GTK_LABEL (label15), TRUE);
  gtk_label_set_justify (GTK_LABEL (label15), GTK_JUSTIFY_LEFT);

  frame15 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame15, "frame15");
  gtk_widget_show (frame15);
  gtk_fixed_put (GTK_FIXED (fixed1), frame15, 496, 210);
  gtk_widget_set_size_request (frame15, 90, 70);

  combo8 = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo8)->popwin),
                     "GladeParentKey", combo8);
  gtk_widget_set_name (combo8, "combo8");
  gtk_widget_show (combo8);
  gtk_container_add (GTK_CONTAINER (frame15), combo8);
  combo8_items = g_list_append (combo8_items, (gpointer) _("Male"));
  combo8_items = g_list_append (combo8_items, (gpointer) _("Female"));
  gtk_combo_set_popdown_strings (GTK_COMBO (combo8), combo8_items);
  g_list_free (combo8_items);

  combo_entry8 = GTK_COMBO (combo8)->entry;
  gtk_widget_set_name (combo_entry8, "combo_entry8");
  gtk_widget_show (combo_entry8);
  gtk_tooltips_set_tip (tooltips, combo_entry8, _("Male/Female"), NULL);
  gtk_editable_set_editable (GTK_EDITABLE (combo_entry8), FALSE);

  label27 = gtk_label_new (_("13. <span foreground=\"#191F7A\">Gender</span>"));
  gtk_widget_set_name (label27, "label27");
  gtk_widget_show (label27);
  gtk_frame_set_label_widget (GTK_FRAME (frame15), label27);
  gtk_label_set_use_markup (GTK_LABEL (label27), TRUE);
  gtk_label_set_justify (GTK_LABEL (label27), GTK_JUSTIFY_LEFT);

  label7 = gtk_label_new (_("Information"));
  gtk_widget_set_name (label7, "label7");
  gtk_widget_show (label7);
  gtk_frame_set_label_widget (GTK_FRAME (frame1), label7);
  gtk_label_set_justify (GTK_LABEL (label7), GTK_JUSTIFY_LEFT);

  hbuttonbox1 = gtk_hbutton_box_new ();
  gtk_widget_set_name (hbuttonbox1, "hbuttonbox1");
  gtk_widget_show (hbuttonbox1);
  gtk_box_pack_start (GTK_BOX (vbox3), hbuttonbox1, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbuttonbox1), 5);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_SPREAD);

  button2 = gtk_button_new_with_mnemonic ("_Exit without saving");
//  button2 = gtk_button_new_from_stock ("gtk-close");
  gtk_widget_set_name (button2, "button2");
  gtk_widget_show (button2);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button2);
  GTK_WIDGET_SET_FLAGS (button2, GTK_CAN_DEFAULT);
  gtk_tooltips_set_tip (tooltips, button2, _("Current data will NOT be saved"), NULL);

  GtkWidget *savexit;
  savexit = gtk_button_new_with_mnemonic ("S_ave and exit");
//  button2 = gtk_button_new_from_stock ("gtk-close");
  gtk_widget_set_name (savexit, "savexit");
  gtk_widget_show (savexit);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), savexit);
  GTK_WIDGET_SET_FLAGS (savexit, GTK_CAN_DEFAULT);
  gtk_tooltips_set_tip (tooltips, savexit, _("Current data will be saved"), NULL);

  button3 = gtk_button_new_from_stock ("gtk-save");
  gtk_widget_set_name (button3, "button3");
  gtk_widget_show (button3);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button3);
  gtk_tooltips_set_tip (tooltips, button3, _("Save a copy to local disk"), NULL);

  /* make the button by default grabbed */
  GTK_WIDGET_SET_FLAGS(button3, GTK_CAN_DEFAULT);
  gtk_widget_grab_default(button3);
  /* ******************** */

  button4 = gtk_button_new_with_mnemonic("_Load Photo");
  gtk_widget_set_name (button4, "button4");
  gtk_widget_show (button4);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button4);
//  GTK_WIDGET_SET_FLAGS (button4, GTK_CAN_DEFAULT);
  gtk_tooltips_set_tip (tooltips, button4, _("Load Photo"), NULL);

  if (ignore_photo) {
    if (GTK_WIDGET_SENSITIVE(button4))
      gtk_widget_set_sensitive(button4, FALSE);
  }

  /*
  alignment1 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_set_name (alignment1, "alignment1");
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (button4), alignment1);

  hbox6 = gtk_hbox_new (FALSE, 2);
  gtk_widget_set_name (hbox6, "hbox6");
  gtk_widget_show (hbox6);
  gtk_container_add (GTK_CONTAINER (alignment1), hbox6);

  image3 = create_pixmap (window2, "online.png");
  gtk_widget_set_name (image3, "image3");
  gtk_widget_show (image3);
  gtk_box_pack_start (GTK_BOX (hbox6), image3, FALSE, FALSE, 0);

  label16 = gtk_label_new_with_mnemonic (_("S_end"));
  gtk_widget_set_name (label16, "label16");
  gtk_widget_show (label16);
  gtk_box_pack_start (GTK_BOX (hbox6), label16, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label16), GTK_JUSTIFY_LEFT);
  */

  g_signal_connect_swapped ((gpointer) window2, "delete_event",
                            G_CALLBACK (on_window2_delete_event),
                            GTK_OBJECT (window2));
  g_signal_connect_swapped ((gpointer) entry2, "changed",
                            G_CALLBACK (on_entry2_changed),
                            GTK_OBJECT (entry2));
  g_signal_connect_swapped ((gpointer) entry3, "changed",
                            G_CALLBACK (on_entry3_changed),
                            GTK_OBJECT (entry3));
  g_signal_connect_swapped ((gpointer) entry4, "changed",
                            G_CALLBACK (on_entry4_changed),
                            GTK_OBJECT (entry4));
  g_signal_connect_swapped ((gpointer) entry9, "changed",
                            G_CALLBACK (on_entry9_changed),
                            GTK_OBJECT (entry9));
  g_signal_connect_swapped ((gpointer) entry8, "changed",
                            G_CALLBACK (on_entry8_changed),
                            GTK_OBJECT (entry8));
  g_signal_connect_swapped ((gpointer) entry5, "changed",
                            G_CALLBACK (on_entry5_changed),
                            GTK_OBJECT (entry5));
  g_signal_connect_swapped ((gpointer) entry6, "changed",
                            G_CALLBACK (on_entry6_changed),
                            GTK_OBJECT (entry6));
  g_signal_connect_swapped ((gpointer) entry7, "changed",
                            G_CALLBACK (on_entry7_changed),
                            GTK_OBJECT (entry7));
  g_signal_connect_swapped ((gpointer) entry10, "changed",
                            G_CALLBACK (on_entry10_changed),
                            GTK_OBJECT (entry10));
  g_signal_connect_swapped ((gpointer) entry11, "changed",
                            G_CALLBACK (on_entry11_changed),
                            GTK_OBJECT (entry11));
  g_signal_connect_swapped ((gpointer) entry12, "changed",
                            G_CALLBACK (on_entry12_changed),
                            GTK_OBJECT (entry12));
  g_signal_connect_swapped ((gpointer) entry13, "changed",
                            G_CALLBACK (on_entry13_changed),
                            GTK_OBJECT (entry13));
  g_signal_connect_swapped ((gpointer) entry14, "changed",
                            G_CALLBACK (on_entry14_changed),
                            GTK_OBJECT (entry14));
  g_signal_connect_swapped ((gpointer) combo_entry9, "changed",
                            G_CALLBACK (on_combo_entry9_changed),
                            GTK_OBJECT (combo_entry9));
  g_signal_connect_swapped ((gpointer) entry15, "changed",
                            G_CALLBACK (on_entry15_changed),
                            GTK_OBJECT (entry15));
  g_signal_connect_swapped ((gpointer) entry16, "changed",
                            G_CALLBACK (on_entry16_changed),
                            GTK_OBJECT (entry16));
  g_signal_connect_swapped ((gpointer) entry17, "changed",
                            G_CALLBACK (on_entry17_changed),
                            GTK_OBJECT (entry17));
  g_signal_connect_swapped ((gpointer) entry18, "changed",
                            G_CALLBACK (on_entry18_changed),
                            GTK_OBJECT (entry18));
  g_signal_connect_swapped ((gpointer) entry19, "changed",
                            G_CALLBACK (on_entry19_changed),
                            GTK_OBJECT (entry19));
  g_signal_connect_swapped ((gpointer) entry20, "changed",
                            G_CALLBACK (on_entry20_changed),
                            GTK_OBJECT (entry20));
  g_signal_connect_swapped ((gpointer) entry21, "changed",
                            G_CALLBACK (on_entry21_changed),
                            GTK_OBJECT (entry21));
  g_signal_connect_swapped ((gpointer) entry22, "changed",
                            G_CALLBACK (on_entry22_changed),
                            GTK_OBJECT (entry22));
  g_signal_connect_swapped ((gpointer) entry23, "changed",
                            G_CALLBACK (on_entry23_changed),
                            GTK_OBJECT (entry23));
  g_signal_connect_swapped ((gpointer) entry24, "changed",
                            G_CALLBACK (on_entry24_changed),
                            GTK_OBJECT (entry24));
  g_signal_connect_swapped ((gpointer) entry25, "changed",
                            G_CALLBACK (on_entry25_changed),
                            GTK_OBJECT (entry25));
  g_signal_connect_swapped ((gpointer) entry26, "changed",
                            G_CALLBACK (on_entry26_changed),
                            GTK_OBJECT (entry26));
  g_signal_connect_swapped ((gpointer) entry27, "changed",
                            G_CALLBACK (on_entry27_changed),
                            GTK_OBJECT (entry27));
  g_signal_connect_swapped ((gpointer) entry28, "changed",
                            G_CALLBACK (on_entry28_changed),
                            GTK_OBJECT (entry28));
  g_signal_connect_swapped ((gpointer) entry29, "changed",
                            G_CALLBACK (on_entry29_changed),
                            GTK_OBJECT (entry29));
  g_signal_connect_swapped ((gpointer) entry30, "changed",
                            G_CALLBACK (on_entry30_changed),
                            GTK_OBJECT (entry30));
  g_signal_connect_swapped ((gpointer) entry31, "changed",
                            G_CALLBACK (on_entry31_changed),
                            GTK_OBJECT (entry31));
  g_signal_connect_swapped ((gpointer) button5, "clicked",
                            G_CALLBACK (on_button5_clicked),
                            GTK_OBJECT (button5));
  g_signal_connect_swapped ((gpointer) combo_entry1, "changed",
                            G_CALLBACK (on_combo_entry1_changed),
                            GTK_OBJECT (combo_entry1));
  g_signal_connect_swapped ((gpointer) combo_entry5, "changed",
                            G_CALLBACK (on_combo_entry5_changed),
                            GTK_OBJECT (combo_entry5));
  g_signal_connect_swapped ((gpointer) combo_entry7, "changed",
                            G_CALLBACK (on_combo_entry7_changed),
                            GTK_OBJECT (combo_entry7));
  g_signal_connect_swapped ((gpointer) combo_entry8, "changed",
                            G_CALLBACK (on_combo_entry8_changed),
                            GTK_OBJECT (combo_entry8));
  g_signal_connect_swapped ((gpointer) button2, "clicked",
                            G_CALLBACK (on_button2_clicked),
                            GTK_OBJECT (button2));
  g_signal_connect_swapped ((gpointer) savexit, "clicked",
                            G_CALLBACK (on_savexit_clicked),
                            GTK_OBJECT (savexit));
  g_signal_connect_swapped ((gpointer) button3, "clicked",
                            G_CALLBACK (on_button3_clicked),
                            GTK_OBJECT (button3));
  g_signal_connect_swapped ((gpointer) button4, "clicked",
                            G_CALLBACK (on_button4_clicked),
                            GTK_OBJECT (button4));

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (window2, window2, "window2");
  GLADE_HOOKUP_OBJECT (window2, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (window2, hbox4, "hbox4");
  GLADE_HOOKUP_OBJECT (window2, image2, "image2");
  GLADE_HOOKUP_OBJECT (window2, label4, "label4");
  GLADE_HOOKUP_OBJECT (window2, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (window2, viewport1, "viewport1");
  GLADE_HOOKUP_OBJECT (window2, frame1, "frame1");
  GLADE_HOOKUP_OBJECT (window2, fixed1, "fixed1");
  GLADE_HOOKUP_OBJECT (window2, entry2, "entry2");
  GLADE_HOOKUP_OBJECT (window2, entry3, "entry3");
  GLADE_HOOKUP_OBJECT (window2, entry4, "entry4");
  GLADE_HOOKUP_OBJECT (window2, frame8, "frame8");
  GLADE_HOOKUP_OBJECT (window2, combo6, "combo6");
  GLADE_HOOKUP_OBJECT (window2, combo_entry6, "combo_entry6");
  GLADE_HOOKUP_OBJECT (window2, label18, "label18");
  GLADE_HOOKUP_OBJECT (window2, table1, "table1");
  GLADE_HOOKUP_OBJECT (window2, label19, "label19");
  GLADE_HOOKUP_OBJECT (window2, label20, "label20");
  GLADE_HOOKUP_OBJECT (window2, entry9, "entry9");
  GLADE_HOOKUP_OBJECT (window2, entry8, "entry8");
  GLADE_HOOKUP_OBJECT (window2, frame3, "frame3");
  GLADE_HOOKUP_OBJECT (window2, entry5, "entry5");
  GLADE_HOOKUP_OBJECT (window2, label12, "label12");
  GLADE_HOOKUP_OBJECT (window2, frame5, "frame5");
  GLADE_HOOKUP_OBJECT (window2, entry6, "entry6");
  GLADE_HOOKUP_OBJECT (window2, label14, "label14");
  GLADE_HOOKUP_OBJECT (window2, frame4, "frame4");
  GLADE_HOOKUP_OBJECT (window2, combo2, "combo2");
  GLADE_HOOKUP_OBJECT (window2, combo_entry2, "combo_entry2");
  GLADE_HOOKUP_OBJECT (window2, label13, "label13");
  GLADE_HOOKUP_OBJECT (window2, frame7, "frame7");
  GLADE_HOOKUP_OBJECT (window2, entry7, "entry7");
  GLADE_HOOKUP_OBJECT (window2, label17, "label17");
  GLADE_HOOKUP_OBJECT (window2, frame9, "frame9");
  GLADE_HOOKUP_OBJECT (window2, fixed2, "fixed2");
  GLADE_HOOKUP_OBJECT (window2, frame11, "frame11");
  GLADE_HOOKUP_OBJECT (window2, entry10, "entry10");
  GLADE_HOOKUP_OBJECT (window2, label23, "label23");
  GLADE_HOOKUP_OBJECT (window2, frame12, "frame12");
  GLADE_HOOKUP_OBJECT (window2, entry11, "entry11");
  GLADE_HOOKUP_OBJECT (window2, label24, "label24");
  GLADE_HOOKUP_OBJECT (window2, frame13, "frame13");
  GLADE_HOOKUP_OBJECT (window2, entry12, "entry12");
  GLADE_HOOKUP_OBJECT (window2, label25, "label25");
  GLADE_HOOKUP_OBJECT (window2, frame14, "frame14");
  GLADE_HOOKUP_OBJECT (window2, hbox7, "hbox7");
  GLADE_HOOKUP_OBJECT (window2, entry13, "entry13");
  GLADE_HOOKUP_OBJECT (window2, label53, "label53");
  GLADE_HOOKUP_OBJECT (window2, entry14, "entry14");
  GLADE_HOOKUP_OBJECT (window2, label26, "label26");
  GLADE_HOOKUP_OBJECT (window2, frame10, "frame10");
  GLADE_HOOKUP_OBJECT (window2, combo7, "combo7");
  GLADE_HOOKUP_OBJECT (window2, combo_entry7, "combo_entry7");
  GLADE_HOOKUP_OBJECT (window2, label22, "label22");
  GLADE_HOOKUP_OBJECT (window2, label21, "label21");
  GLADE_HOOKUP_OBJECT (window2, frame16, "frame16");
  GLADE_HOOKUP_OBJECT (window2, combo9, "combo9");
  GLADE_HOOKUP_OBJECT (window2, combo_entry9, "combo_entry9");
  GLADE_HOOKUP_OBJECT (window2, label28, "label28");
  GLADE_HOOKUP_OBJECT (window2, frame17, "frame17");
  GLADE_HOOKUP_OBJECT (window2, table2, "table2");
  GLADE_HOOKUP_OBJECT (window2, frame25, "frame25");
  GLADE_HOOKUP_OBJECT (window2, entry15, "entry15");
  GLADE_HOOKUP_OBJECT (window2, label37, "label37");
  GLADE_HOOKUP_OBJECT (window2, frame26, "frame26");
  GLADE_HOOKUP_OBJECT (window2, entry16, "entry16");
  GLADE_HOOKUP_OBJECT (window2, label38, "label38");
  GLADE_HOOKUP_OBJECT (window2, frame27, "frame27");
  GLADE_HOOKUP_OBJECT (window2, entry17, "entry17");
  GLADE_HOOKUP_OBJECT (window2, label39, "label39");
  GLADE_HOOKUP_OBJECT (window2, frame28, "frame28");
  GLADE_HOOKUP_OBJECT (window2, entry18, "entry18");
  GLADE_HOOKUP_OBJECT (window2, label40, "label40");
  GLADE_HOOKUP_OBJECT (window2, frame29, "frame29");
  GLADE_HOOKUP_OBJECT (window2, entry19, "entry19");
  GLADE_HOOKUP_OBJECT (window2, label41, "label41");
  GLADE_HOOKUP_OBJECT (window2, frame30, "frame30");
  GLADE_HOOKUP_OBJECT (window2, entry20, "entry20");
  GLADE_HOOKUP_OBJECT (window2, label42, "label42");
  GLADE_HOOKUP_OBJECT (window2, frame31, "frame31");
  GLADE_HOOKUP_OBJECT (window2, entry21, "entry21");
  GLADE_HOOKUP_OBJECT (window2, label43, "label43");
  GLADE_HOOKUP_OBJECT (window2, frame32, "frame32");
  GLADE_HOOKUP_OBJECT (window2, entry22, "entry22");
  GLADE_HOOKUP_OBJECT (window2, label44, "label44");
  GLADE_HOOKUP_OBJECT (window2, frame33, "frame33");
  GLADE_HOOKUP_OBJECT (window2, entry23, "entry23");
  GLADE_HOOKUP_OBJECT (window2, label45, "label45");
  GLADE_HOOKUP_OBJECT (window2, frame34, "frame34");
  GLADE_HOOKUP_OBJECT (window2, entry24, "entry24");
  GLADE_HOOKUP_OBJECT (window2, label46, "label46");
  GLADE_HOOKUP_OBJECT (window2, frame35, "frame35");
  GLADE_HOOKUP_OBJECT (window2, entry25, "entry25");
  GLADE_HOOKUP_OBJECT (window2, label47, "label47");
  GLADE_HOOKUP_OBJECT (window2, frame36, "frame36");
  GLADE_HOOKUP_OBJECT (window2, entry26, "entry26");
  GLADE_HOOKUP_OBJECT (window2, label48, "label48");
  GLADE_HOOKUP_OBJECT (window2, frame37, "frame37");
  GLADE_HOOKUP_OBJECT (window2, entry27, "entry27");
  GLADE_HOOKUP_OBJECT (window2, label49, "label49");
  GLADE_HOOKUP_OBJECT (window2, frame38, "frame38");
  GLADE_HOOKUP_OBJECT (window2, entry28, "entry28");
  GLADE_HOOKUP_OBJECT (window2, label50, "label50");
  GLADE_HOOKUP_OBJECT (window2, frame39, "frame29");
  GLADE_HOOKUP_OBJECT (window2, entry29, "entry29"); /* extra 1 */
  GLADE_HOOKUP_OBJECT (window2, entry30, "entry30"); /* extra 2 */
  GLADE_HOOKUP_OBJECT (window2, entry31, "entry31");
  GLADE_HOOKUP_OBJECT (window2, label51, "label51");
  GLADE_HOOKUP_OBJECT (window2, frame40, "frame40");
  GLADE_HOOKUP_OBJECT (window2, button5, "button5");
  GLADE_HOOKUP_OBJECT (window2, label52, "label52");
  GLADE_HOOKUP_OBJECT (window2, label29, "label29");
  GLADE_HOOKUP_OBJECT (window2, label8, "label8");
  GLADE_HOOKUP_OBJECT (window2, label9, "label9");
  GLADE_HOOKUP_OBJECT (window2, label10, "label10");
  GLADE_HOOKUP_OBJECT (window2, frame2, "frame2");
  GLADE_HOOKUP_OBJECT (window2, combo1, "combo1");
  GLADE_HOOKUP_OBJECT (window2, combo_entry1, "combo_entry1");
  GLADE_HOOKUP_OBJECT (window2, label11, "label11");
  GLADE_HOOKUP_OBJECT (window2, frame6, "frame6");
  GLADE_HOOKUP_OBJECT (window2, hbox5, "hbox5");
  GLADE_HOOKUP_OBJECT (window2, combo3, "combo3");
  GLADE_HOOKUP_OBJECT (window2, combo_entry3, "combo_entry3");
  GLADE_HOOKUP_OBJECT (window2, combo4, "combo4");
  GLADE_HOOKUP_OBJECT (window2, combo_entry4, "combo_entry4");
//  GLADE_HOOKUP_OBJECT (window2, combo5, "combo5");
  GLADE_HOOKUP_OBJECT (window2, combo_entry5, "combo_entry5");
  GLADE_HOOKUP_OBJECT (window2, label15, "label15");
  GLADE_HOOKUP_OBJECT (window2, frame15, "frame15");
  GLADE_HOOKUP_OBJECT (window2, combo8, "combo8");
  GLADE_HOOKUP_OBJECT (window2, combo_entry8, "combo_entry8");
  GLADE_HOOKUP_OBJECT (window2, label27, "label27");
  GLADE_HOOKUP_OBJECT (window2, label7, "label7");
  GLADE_HOOKUP_OBJECT (window2, hbuttonbox1, "hbuttonbox1");
  GLADE_HOOKUP_OBJECT (window2, button2, "button2");
  GLADE_HOOKUP_OBJECT (window2, button3, "button3");
  GLADE_HOOKUP_OBJECT (window2, button4, "button4");
//  GLADE_HOOKUP_OBJECT (window2, alignment1, "alignment1");
//  GLADE_HOOKUP_OBJECT (window2, hbox6, "hbox6");
//  GLADE_HOOKUP_OBJECT (window2, image3, "image3");
//  GLADE_HOOKUP_OBJECT (window2, label16, "label16");
  GLADE_HOOKUP_OBJECT_NO_REF (window2, tooltips, "tooltips");

  /* modified by ayub */
  /* load college information */
  /* entry8 -> college code entry
     entry9 -> college name entry
   */
#ifdef ____NO____
  update_college_code_name(GTK_ENTRY(entry8), GTK_ENTRY(entry9));
#endif
  char colcode[5];
  sprintf(colcode, "%04d", all_col_code);
  gtk_entry_set_text(GTK_ENTRY(entry8), colcode);
  gtk_editable_set_editable(GTK_EDITABLE(entry8), FALSE);
  gtk_entry_set_text(GTK_ENTRY(entry9), all_col_name);
  gtk_editable_set_editable(GTK_EDITABLE(entry9), FALSE);

  int subentry = 0;
  subentry = load_sub_common(subentrymap[GROUPS][1], &subcodes[subentry]);
//  num_all_common_sub = subentry;
  /* Science is the default group; filling common science subjects */
  subentry += load_sub_common(subentrymap[0][1], &subcodes[subentry]);
  /* modification ends */
  gtk_widget_grab_focus(entry2); /* focus student's name */

  return window2;
}

