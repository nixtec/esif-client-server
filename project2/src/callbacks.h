#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_

/* GNU headers always checks for symbols before
 * inclusion, so you need not manually check for
 * them
 */
#include "includes.h"

gboolean
on_window1_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_window2_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void q_okbutton_clicked(GtkButton *button, gpointer user_data);

void
on_entry2_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);
void
on_entry3_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);
void
on_entry4_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry8_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);
void
on_entry9_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry9_changed                  (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry8_changed                  (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry5_changed                  (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry6_changed                  (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry7_changed                  (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry10_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry11_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry12_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry13_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry14_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_combo_entry9_changed                (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry15_changed                     (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry16_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry17_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry18_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry19_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry20_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry21_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry22_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry23_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry24_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry25_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry26_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry27_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry28_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry29_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry30_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_entry31_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_combo_entry1_changed                (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_combo_entry5_changed            (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_combo_entry7_changed            (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_combo_entry8_changed                (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data);
void
on_savexit_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data);


void
on_cancelbutton1_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_okbutton1_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_radiobutton1_toggled          (GtkToggleButton *button,
    					gpointer user_data);

void
on_radiobutton2_toggled          (GtkToggleButton *button,
    					gpointer user_data);

void
photo_button_clicked               (GtkButton       *button,
                                        gpointer         user_data);
gboolean
photo_image_pressed               (GtkWidget *widget, GdkEventButton *event,
                                        gpointer         user_data);

void
on_fa_name_toggled                     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_mo_name_toggled                     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_birthdate_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_totlist_toggled                     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_prev_year_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_prev_roll_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_prev_reg_toggled                    (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_prev_sess_toggled                    (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_ignore_close_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_photo_toggled                       (GtkToggleButton *togglebutton,
                                        gpointer         user_data);


#endif /* _CALLBACKS_H_ */
