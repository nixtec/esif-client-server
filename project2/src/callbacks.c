#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#include "defines.h"        /* ACCEPTCHARS */
#include "myfuncs.h"        /* my own function definitions */
#include "editor.h"         /* refresh_editor_list(), etc */

const char *subentrymap[][2] = {
  { "Science", "sub_common_sc.txt" },
  { "Social Science", "sub_common_so_sc.txt" },
  { "Arts", "sub_common_arts.txt" },
  { "Commerce", "sub_common_com.txt" },
  { "Agriculture", "sub_common_agri.txt" },
  { "Home Economics", "sub_common_home_eco.txt" },
  { "Islamic Studies", "sub_common_is_st.txt" },
  { "Music", "sub_common_music.txt" },
  { "Business", "sub_common_busi.txt" },
  { NULL, "sub_common_all.txt" } /* functions will not check this */
};

static GtkWidget *window1; /* introduction window */
static GtkWidget *window2; /* form window */
static GtkWidget *q_window; /* query window */

static GtkWidget *window; /* syllabus window */
static GtkWidget *confirm; /* confirm window (dialog) */



gboolean
on_window1_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  gtk_main_quit();
  return FALSE;
}


void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

  window1 = lookup_widget(GTK_WIDGET(user_data), "window1");
  if (GTK_WIDGET_VISIBLE(window1))
      gtk_widget_hide(window1);
  gtk_object_destroy(GTK_OBJECT(window1)); /* no longer necessary */

  if (q_window == NULL)
    q_window = query();
  if (!GTK_WIDGET_VISIBLE(q_window))
    gtk_widget_show(q_window);

}

void q_okbutton_clicked(GtkButton *button, gpointer user_data)
{
  GtkWidget *ptr;
  char buf[5];

  ptr = lookup_widget(GTK_WIDGET(button), "entry1");
  if (!ptr)
    return;

  /* set global college code */
  strcpy(buf, gtk_entry_get_text(GTK_ENTRY(ptr)));
  all_col_code = (int) strtol(buf, (char **) NULL, 10);
  sprintf(all_colcode, "%04d", all_col_code);

  ptr = lookup_widget(GTK_WIDGET(button), "label4");
  if (!ptr)
    return;
  /* set global college name */
  strcpy(all_col_name, gtk_label_get_text(GTK_LABEL(ptr)));

  remove_col_hash_table(); /* it's no longer needed */

  ptr = lookup_widget(GTK_WIDGET(button), "entry2");

  /* set global session */
  char *s = strdup(gtk_entry_get_text(GTK_ENTRY(ptr)));
  int i = strspn(s, ACCEPTNUMBERS);
  *(s+i) = 0; /* ignore '-' */
  strcpy(all_session, s);
  free(s);

  init_ignored();

  if (window2 == NULL)
    window2 = create_window2();
  if (!GTK_WIDGET_VISIBLE (window2))
    gtk_widget_show(window2);

  /* disable some widgets for S. S. C */
  if (is_ssc) {
    ptr = lookup_widget(window2, "combo7");
    if (ptr)
      gtk_widget_set_sensitive(ptr, FALSE);

    ptr = lookup_widget(window2, "entry10");
    if (ptr)
      gtk_widget_set_sensitive(ptr, FALSE);

    ptr = lookup_widget(window2, "entry11");
    if (ptr)
      gtk_widget_set_sensitive(ptr, FALSE);

    ptr = lookup_widget(window2, "entry12");
    if (ptr)
      gtk_widget_set_sensitive(ptr, FALSE);

    ptr = lookup_widget(window2, "entry13");
    if (ptr)
      gtk_widget_set_sensitive(ptr, FALSE);

    ptr = lookup_widget(window2, "entry14");
    if (ptr)
      gtk_widget_set_sensitive(ptr, FALSE);

  }



  fix_info_path(); /* create path where informations will be saved */

  load_editor();

  gtk_object_destroy(GTK_OBJECT(q_window));
  q_window = NULL;

}


gboolean
on_window2_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  if (!confirm) {
    confirm = confirm_dialog();
  }
  if (!GTK_WIDGET_VISIBLE(confirm))
    gtk_widget_show(confirm);

//#ifdef _WIN32
//  WSACleanup(); /* cleanup winsock.dll */
//#endif

//  gtk_main_quit();
  return TRUE; /* don't destroy */
}



void
on_entry2_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  char *up;

  PRINTF("on_entry2_changed\n");

  /* make it uppercase */
  up = strdup(strupper(gtk_entry_get_text(GTK_ENTRY(editable))));
  PRINTF("UPPERCASE : %s\n", up);

  size_t validchars;
  validchars = strspn(up, ACCEPTCHARS);
  *(up+validchars) = '\0'; /* dont' accept invalid chars */

  gtk_entry_set_text(GTK_ENTRY(editable), up);

  free(up);
  /* update gender using this entry value */
  update_gender(GTK_ENTRY(editable));

}

void
on_entry3_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  char *up;
  size_t validchars;

  PRINTF("on_entry2_changed\n");

  /* make it uppercase */
  up = strdup(strupper(gtk_entry_get_text(GTK_ENTRY(editable))));
  PRINTF("UPPERCASE : %s\n", up);

  validchars = strspn(up, ACCEPTCHARS);
  *(up+validchars) = '\0'; /* dont' accept invalid chars */

  gtk_entry_set_text(GTK_ENTRY(editable), up);
  free(up);

}

void
on_entry4_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  char *up;
  size_t validchars;

  PRINTF("on_entry2_changed\n");

  /* make it uppercase */
  up = strdup(strupper(gtk_entry_get_text(GTK_ENTRY(editable))));
  PRINTF("UPPERCASE : %s\n", up);

  validchars = strspn(up, ACCEPTCHARS);
  *(up+validchars) = '\0'; /* dont' accept invalid chars */

  gtk_entry_set_text(GTK_ENTRY(editable), up);
  free(up);

}




void
on_entry9_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{

  char *up;

  PRINTF("on_entry9_changed\n");

  /* make it uppercase */
  up = strdup(strupper(gtk_entry_get_text(GTK_ENTRY(editable))));
  PRINTF("UPPERCASE : %s\n", up);

  gtk_entry_set_text(GTK_ENTRY(editable), up);
  free(up);

}


void
on_entry8_changed                  (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;
  int len;

  PRINTF("***** on_entry8_changed *****\n");
  entry = lookup_widget(GTK_WIDGET(user_data), "entry8");
  if (entry == NULL)
    return;

  len = strlen(gtk_entry_get_text(GTK_ENTRY(entry)));
  if (len < 4) { /* jump to next entry as soon as 4 chars entered */
    PRINTF("len = %d\n", len);
    return;
  }

  entry = lookup_widget(GTK_WIDGET(user_data), "entry9");
  if (entry) {
    PRINTF("entry9 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}


void
on_entry5_changed                  (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;
  int len;

  PRINTF("***** on_entry5_changed *****\n");
  entry = lookup_widget(GTK_WIDGET(user_data), "entry5");
  if (entry == NULL)
    return;

  len = strlen(gtk_entry_get_text(GTK_ENTRY(entry)));
  if (len < 4) { /* jump to next entry as soon as 4 chars entered */
    PRINTF("len = %d\n", len);
    return;
  }

  entry = lookup_widget(GTK_WIDGET(user_data), "entry6");
  if (entry) {
    PRINTF("entry6 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}


void
on_entry6_changed                  (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;
//  int len;

  PRINTF("***** on_entry6_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 4) {
    free(val);
    return;
  }
  free(val);


  entry = lookup_widget(GTK_WIDGET(user_data), "combo_entry3");
  if (entry) {
    PRINTF("combo_entry3 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry7_changed                  (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GtkWidget *entry;

  PRINTF("***** on_entry7_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 4) {
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "combo_entry6");
  if (entry) {
    PRINTF("combo_entry6 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }


}



void
on_entry10_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry10_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 4) {
    free(val);
    return;
  }
  free(val);


  entry = lookup_widget(GTK_WIDGET(user_data), "entry11");
  if (entry) {
    PRINTF("entry11 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry11_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry11_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < gtk_entry_get_max_length(GTK_ENTRY(editable))) {
    free(val);
    return;
  }
  free(val);


  entry = lookup_widget(GTK_WIDGET(user_data), "entry12");
  if (entry) {
    PRINTF("entry12 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }
}



void
on_entry12_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GtkWidget *entry;

  PRINTF("***** on_entry12_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < gtk_entry_get_max_length(GTK_ENTRY(editable))) {
    free(val);
    return;
  }
  free(val);


  entry = lookup_widget(GTK_WIDGET(user_data), "entry13");
  if (entry) {
    PRINTF("entry13 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry13_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;
  char sess_next[5];
  int session;

  PRINTF("***** on_entry13_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 4) {
    free(val);
    return;
  }
  free(val);


  session = atoi(gtk_entry_get_text(GTK_ENTRY(editable)));
  session++;
  sprintf(sess_next, "%d", session);

  /* next entry should be automatically filled up */
  entry = lookup_widget(GTK_WIDGET(user_data), "entry14");
  if (entry) {
    gtk_entry_set_text(GTK_ENTRY(entry), sess_next);
  }

  entry = lookup_widget(GTK_WIDGET(user_data), "combo_entry8");
  if (entry) {
    PRINTF("combo_entry8 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry14_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry14_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 4) {
    free(val);
    return;
  }
  free(val);


  entry = lookup_widget(GTK_WIDGET(user_data), "combo_entry8");
  if (entry) {
    PRINTF("combo_entry8 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}


void
on_combo_entry9_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{

}


/* user MUST type an extra space to go to next entry */

void
on_entry15_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry15_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry15");
  if (entry == NULL)
    return;

  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry16");
  if (entry) {
    PRINTF("entry16 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}


void
on_entry16_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry16_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry16");
  if (entry == NULL)
    return;

  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry17");
  if (entry) {
    PRINTF("entry17 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry17_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry17_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry17");
  if (entry == NULL)
    return;

  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry18");
  if (entry) {
    PRINTF("entry18 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry18_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry18_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry18");
  if (entry == NULL)
    return;

  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry19");
  if (entry) {
    PRINTF("entry19 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry19_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry19_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry19");
  if (entry == NULL)
    return;


  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry20");
  if (entry) {
    PRINTF("entry20 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry20_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry20_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry20");
  if (entry == NULL)
    return;

  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry21");
  if (entry) {
    PRINTF("entry21 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry21_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry21_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry21");
  if (entry == NULL)
    return;


  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry22");
  if (entry) {
    PRINTF("entry22 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}




void
on_entry22_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry22_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry22");
  if (entry == NULL)
    return;


  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry23");
  if (entry) {
    PRINTF("entry23 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}


void
on_entry23_changed                    (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry23_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry23");
  if (entry == NULL)
    return;

  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry24");
  if (entry) {
    PRINTF("entry24 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry24_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry24_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry24");
  if (entry == NULL)
    return;

  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry25");
  if (entry) {
    PRINTF("entry25 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry25_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry25_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry25");
  if (entry == NULL)
    return;

  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry26");
  if (entry) {
    PRINTF("entry26 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry26_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry26_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry26");
  if (entry == NULL)
    return;

  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry27");
  if (entry) {
    PRINTF("entry27 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry27_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry27_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry27");
  if (entry == NULL)
    return;


  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry28");
  if (entry) {
    PRINTF("entry28 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



void
on_entry28_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry28_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry28");
  if (entry == NULL)
    return;

  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry29");
  if (entry) {
    PRINTF("entry29 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}


void
on_entry29_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry29_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry29");
  if (entry == NULL)
    return;

  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry30");
  if (entry) {
    PRINTF("entry30 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}

void
on_entry30_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry30_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry30");
  if (entry == NULL)
    return;

  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry31");
  if (entry) {
    PRINTF("entry31 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}

void
on_entry31_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget *entry;

  PRINTF("***** on_entry31_changed *****\n");
  char *val;
  int i;
  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 3) {
    set_sub_tip(GTK_WIDGET(editable), 0); /* set '3 digits' tip */
    free(val);
    return;
  }
  free(val);

  entry = lookup_widget(GTK_WIDGET(user_data), "entry31");
  if (entry == NULL)
    return;

  /* sets subject name as tip */
  int codeval = strtol(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 10);
  set_sub_tip(entry, codeval);

  entry = lookup_widget(GTK_WIDGET(user_data), "button3");
  if (entry) {
    PRINTF("button3 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}



/* subject code index button */
void
on_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  if ((window != NULL) && GTK_WIDGET_VISIBLE(window))
    gtk_widget_hide(window);
  else if (window == NULL)
    window = view_syllabus();

  if (!GTK_WIDGET_VISIBLE(window))
    gtk_widget_show(window);

//    window = do_list_store(window);

}


void
on_combo_entry1_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{

  GtkWidget **subcodes;
  GtkWidget *entry;
  char *entrystr;
  int i = 0;
  int subentry = 0;

  PRINTF("on_combo_entry1_changed\n");
  subcodes = (GtkWidget **) g_object_get_data(G_OBJECT(user_data), "subentries");
  if (subcodes == NULL) {
    PRINTF("Data from object not available\n");
    return;
  }

//  PRINTF("combo_entry1->lookup  ...");
  entry = GTK_WIDGET(editable);
  /*
  entry = lookup_widget(GTK_WIDGET(editable), "combo_entry1");
  if (!entry)
    return;
  else
    PRINTF("found\n");
   */

  entrystr = strdup(gtk_entry_get_text(GTK_ENTRY(entry)));

  /* check for group */
  for (i=0; i<GROUPS; i++) {
    if (!strcmp(subentrymap[i][0], entrystr)) {
      PRINTF("matching %s at index %d\n", entrystr, i);
      break;
    }
  }

  /*
  PRINTF("subentrymap[i][0] = %s\n", subentrymap[i][0]);
  PRINTF("subentrymap[i][1] = %s\n", subentrymap[i][1]);
  */

  subentry = load_sub_common(subentrymap[GROUPS][1], &subcodes[subentry]);
  /* now fill the entry common for subentrymap[i][0] group */
//  subentry = num_all_common_sub;
  subentry += load_sub_common(subentrymap[i][1], &subcodes[subentry]);
  clear_sub_entry(&subcodes[subentry]); /* clear rest of the entries */

  gtk_widget_grab_focus(entry);

  free(entrystr);
}


void
on_combo_entry5_changed            (GtkEditable     *editable,
                                        gpointer         user_data)
{

  char *val;
  int i;
  GtkWidget *entry;

  PRINTF("***** on_combo_entry5_changed *****\n");

  val = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  i = strspn(val, ACCEPTNUMBERS);
  *(val+i) = '\0';
  gtk_entry_set_text(GTK_ENTRY(editable), val);
  if (strlen(val) < 4) {
    free(val);
    return;
  }
  free(val);


  /*
  int len;
  entry = lookup_widget(GTK_WIDGET(user_data), "combo_entry5");
  if (entry == NULL)
    return;

  len = strlen(gtk_entry_get_text(GTK_ENTRY(entry)));
  if (len < 4) {
    PRINTF("len = %d\n", len);
    return;
  }
  */

  entry = lookup_widget(GTK_WIDGET(user_data), "entry7");
  if (entry) {
    PRINTF("entry7 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }

}


void
on_combo_entry7_changed            (GtkEditable     *editable,
                                        gpointer         user_data)
{

  /*
  GtkWidget *entry;
  int len;

  PRINTF("***** on_combo_entry5_changed *****\n");
  entry = lookup_widget(GTK_WIDGET(user_data), "combo_entry5");
  if (entry == NULL)
    return;

  len = strlen(gtk_entry_get_text(GTK_ENTRY(entry)));
  if (len < 4) {
    PRINTF("len = %d\n", len);
    return;
  }

  entry = lookup_widget(GTK_WIDGET(user_data), "entry7");
  if (entry) {
    PRINTF("entry7 found\n");
    gtk_widget_grab_focus(GTK_WIDGET(entry));
  }
  */
  GtkWidget *ptr;
  char *temp;

  temp = strdup(gtk_entry_get_text(GTK_ENTRY(editable)));
  if (!strcmp(temp, "Technical") || !strcmp(temp, "Others")) {
    ptr = lookup_widget(GTK_WIDGET(user_data), "entry11");
    if (!ptr)
      goto free_mem;

    gtk_entry_set_max_length(GTK_ENTRY(ptr), 10);

    ptr = lookup_widget(GTK_WIDGET(user_data), "entry12");
    if (!ptr)
      goto free_mem;
    gtk_entry_set_max_length(GTK_ENTRY(ptr), 10);
  }
  else {
    ptr = lookup_widget(GTK_WIDGET(user_data), "entry11");
    if (!ptr)
      goto free_mem;

    gtk_entry_set_max_length(GTK_ENTRY(ptr), 6);

    ptr = lookup_widget(GTK_WIDGET(user_data), "entry12");
    if (!ptr)
      goto free_mem;
    gtk_entry_set_max_length(GTK_ENTRY(ptr), 6);
  }

free_mem:
  free(temp);

}

void
on_combo_entry8_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{

}


void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

#ifdef _WIN32
  WSACleanup();
#endif

  gtk_main_quit();
}


void
on_savexit_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

  int ok = 1;
  ok = verify_form(user_data, TRUE); /* verify and clear */
  if (ok != E_NO_ERROR) {
    show_error(ok);
    return;
  }
  else {
    on_button2_clicked(NULL, NULL);
//    refresh_editor_list();
  }
}

void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

  int ok = 1;
  ok = verify_form(user_data, TRUE); /* verify and clear */
  if ((ok != E_NO_ERROR) || (ok != E_NO_ERROR_FAILED))
    show_error(ok);
  if (ok == E_NO_ERROR)
    refresh_editor_list();
}


/* load photo */
void
on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

  GtkWidget *ptr;

  PRINTF("***on_button4_clicked***\n");
  ptr = lookup_widget(GTK_WIDGET(button), "photo_frame");
  if (ptr) {
    add_photo(ptr);
  }

}

void
on_cancelbutton1_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{

  GtkWidget *ptr = lookup_widget(GTK_WIDGET(button), "dialog1");
  if (ptr)
    gtk_widget_hide(ptr);
}


void
on_okbutton1_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
#ifdef _WIN32
  WSACleanup(); /* cleanup winsock.dll */
#endif

  gtk_main_quit();


}

/* radiobutton1 => S. S. C */
void
on_radiobutton1_toggled          (GtkToggleButton *button,
    					gpointer user_data)
{
  PRINTF("***on_radiobutton1_toggled***\n");
  is_ssc = 1;

}

void
on_radiobutton2_toggled          (GtkToggleButton *button,
    					gpointer user_data)
{

  PRINTF("***on_radiobutton2_toggled***\n");
  is_ssc = 0;

}

void
photo_button_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *ptr;

  ptr = lookup_widget(GTK_WIDGET(button), "photo_frame");

  if (ptr)
    add_photo(ptr);

}

gboolean
photo_image_pressed               (GtkWidget *widget, GdkEventButton *event,
                                        gpointer         user_data)
{
  GtkWidget *ptr;

  PRINTF("***photo_image_clicked***\n");

  ptr = lookup_widget(widget, "photo_frame");

  if (ptr)
    add_photo(ptr);

  return FALSE;
}

/* initialized ignored value

  ignore_fa_name = 0;
  ignore_mo_name = 0;
  ignore_birthdate = 0;
  ignore_totlist = 0;
  ignore_prev_year = 0;
  ignore_prev_roll = 0;
  ignore_prev_reg = 0;
  ignore_prev_sess = 0;
  ignore_photo = 0;

*/

/* callbacks */
void
on_fa_name_toggled                     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkWidget *ptr;
  gboolean truth;

  ptr = lookup_widget(window2, "entry3");
  if (ptr) {
    truth = 1 ^ GTK_WIDGET_SENSITIVE(ptr);
    gtk_widget_set_sensitive(ptr, truth);
    ignore_fa_name ^= 1;
  }

}


void
on_mo_name_toggled                     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

  GtkWidget *ptr;
  gboolean truth;

  ptr = lookup_widget(window2, "entry4");
  if (ptr) {
    truth = 1 ^ GTK_WIDGET_SENSITIVE(ptr);
    gtk_widget_set_sensitive(ptr, truth);
    ignore_mo_name ^= 1;
  }

}


void
on_birthdate_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

  GtkWidget *ptr;
  gboolean truth;

  ptr = lookup_widget(window2, "combo5");
  if (ptr) {
    truth = 1 ^ GTK_WIDGET_SENSITIVE(ptr);
    gtk_widget_set_sensitive(ptr, truth);
    ignore_birthdate ^= 1;
  }


}


void
on_totlist_toggled                     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

  GtkWidget *ptr;
  gboolean truth;

  ptr = lookup_widget(window2, "entry7");
  if (ptr) {
    truth = 1 ^ GTK_WIDGET_SENSITIVE(ptr);
    gtk_widget_set_sensitive(ptr, truth);
    ignore_totlist ^= 1;
  }


}


void
on_prev_year_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkWidget *ptr;
  gboolean truth;

  if (is_ssc) /* for HSC only */
    return;

  ptr = lookup_widget(window2, "entry10");
  if (ptr) {
    truth = 1 ^ GTK_WIDGET_SENSITIVE(ptr);
    gtk_widget_set_sensitive(ptr, truth);
    ignore_prev_year ^= 1;
  }


}


void
on_prev_roll_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

  GtkWidget *ptr;
  gboolean truth;

  if (is_ssc) /* for HSC only */
    return;

  ptr = lookup_widget(window2, "entry11");
  if (ptr) {
    truth = 1 ^ GTK_WIDGET_SENSITIVE(ptr);
    gtk_widget_set_sensitive(ptr, truth);
    ignore_prev_roll ^= 1;
  }

}


void
on_prev_reg_toggled                    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

  GtkWidget *ptr;
  gboolean truth;

  if (is_ssc) /* for HSC only */
    return;

  ptr = lookup_widget(window2, "entry12");
  if (ptr) {
    truth = 1 ^ GTK_WIDGET_SENSITIVE(ptr);
    gtk_widget_set_sensitive(ptr, truth);
    ignore_prev_reg ^= 1;
  }

}

void
on_prev_sess_toggled                    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

  GtkWidget *ptr;
  gboolean truth;

  if (is_ssc) /* for HSC only */
    return;

  ptr = lookup_widget(window2, "entry13");
  if (ptr) {
    truth = 1 ^ GTK_WIDGET_SENSITIVE(ptr);
    gtk_widget_set_sensitive(ptr, truth);
  }
  ptr = lookup_widget(window2, "entry14");
  if (ptr) {
    truth = 1 ^ GTK_WIDGET_SENSITIVE(ptr);
    gtk_widget_set_sensitive(ptr, truth);
  }

  ignore_prev_sess ^= 1;
}


void
on_ignore_close_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{

  GtkWidget *ptr;

  ptr = lookup_widget(GTK_WIDGET(button), "window1");
  if (ptr)
    gtk_widget_hide(ptr);

}


void
on_photo_toggled                       (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

  GtkWidget *ptr;
  gboolean truth;

  ptr = lookup_widget(window2, "button4");
  if (ptr) {
    truth = 1 ^ GTK_WIDGET_SENSITIVE(ptr);
    gtk_widget_set_sensitive(ptr, truth);
  }

  ignore_photo ^= 1;

}

