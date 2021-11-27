#ifndef _MYFUNCS_H_
#define _MYFUNCS_H_

#include "includes.h"

/* a group can have subject codes of the appropriate
 * set of suject codes
 * to be loaded from the corresponding filenames
 */

/* read-only data may be shared among all */
extern const char *groupfiles[][3]; /* defined in myfuncs.c */

int verify_form(gpointer data, gboolean clear);
int save_form(gpointer data);
char *apply_forms(const char *host, const char *dir, const char *college, const char *session);
int verify_number(const char *str);
int verify_sub_code(const char *code, const char **codes);
int verify_subject_codes(GtkWidget *, const char **codes);
int verify_subject_codes_by_group(GtkWidget *widget,
    const char **codes, const char *filename, const char *fourthsubfile);
void print_slist_int(GSList *); /* print singly linked list containing int */
int verify_name(const char *str);
int verify_session(const char *str);
int verify_year(const char *str);
int verify_date(int dd, char *Mon, int yyyy); /* date => dd-Mon-yyyy */
char *strupper(const char *str);
void ask_pass(void);
void pass_ok_clicked(GtkButton *, gpointer);
void pass_cancel_clicked(GtkButton *, gpointer);
GtkWidget* online_transfer_window (void);
void on_online_cancel_clicked(GtkButton *button, gpointer data);
void on_online_ok_clicked(GtkButton *button, gpointer data);
void handle_signal(int signo);

GtkWidget *do_list_store(GtkWidget *window); /* window may be NULL */
GtkTreeModel *create_model(void);
void display_error(const char *str);
void show_error(int number);
void add_columns(GtkTreeView *tree);

GHashTable *create_pair_hash_table(const char *filename);
int paircheck(GSList *filled_codes);

int apply_form(const char *filename); /* apply this form */
void show_message(const char *str);

void fix_info_path(void);
void load_college_info(const char *filename, GtkWidget *code, GtkWidget *name);
int load_sub_common(const char *filename, GtkWidget *ent[]);
void clear_sub_entry(GtkWidget *ent[]); /* clear rest of the entries */

/* create a hash table from a given subject code file */
GHashTable *create_sub_hash_table(const char *filename);
const char *lookup_subject_by_code(unsigned int);
void set_sub_tip(GtkWidget *, unsigned int code);
void update_gender(GtkEntry *entry2); /* update gender using student's entry */

int authenticate(const char *username, const char *pass);

GHashTable *create_col_hash_table(const char *filename);
void update_college_code_name(GtkEntry *code, GtkEntry *name);
char *lookup_college_by_code(const char *code);
GtkWidget *query(void);
void q_col_entry_changed(GtkEditable *entry, gpointer data);
void q_sess_entry_changed(GtkEditable *entry, gpointer data);
void q_okbutton_clicked(GtkButton *button, gpointer data);
void remove_col_hash_table(void);
gboolean on_q_window_delete(GtkWidget *widget, GdkEvent *event, gpointer data);

GtkWidget* view_syllabus (void);
GtkWidget* confirm_dialog (void);

/* global externals:
 * all will share this
 */
/* number of common subject for all */
extern unsigned int num_all_common_sub;  /* defined in interface.c */

/* subject code widgets */
//extern GtkWidget *subcodes[]; /* defined in interface.c */
/* group=> subject code file mapping */
extern const char *subentrymap[][2]; /* defined in callbacks.c */

extern int all_col_code; /* defined in myfuncs.c */
extern char all_colcode[]; /* defined in myfuncs.c */
extern char all_col_name[]; /* defined in myfuncs.c */
extern char all_session[]; /* defined in myfuncs.c */

/* buffer for directory of information (all must use it) */
/* will be initialized during load */
extern char infodirname[]; /* defined in editor.c */

/* username is all_col_code[] */
//extern char pass[]; /* login password; defined in myfuncs.c */
extern char error_msg[]; /* defined in myfuncs.c */
extern int is_ssc; /* is ssc selected? (defined in myfuncs.c) */


/* pointer to the subject code hash table */
//extern GHashTable *subhtable; /* defined in myfuncs.c */

#endif /* _MYFUNCS_H_ */
