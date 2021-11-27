/* editor.h
 */
#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "includes.h"

/* editor view's column enumerations */
enum {
  COLUMN_ROLL,     /* class roll */
  COLUMN_ST_NAME,  /* student's name */
  COLUMN_SECTION,  /* section */
  COLUMN_GENDER,  /* Male/Female */
  COLUMN_TOTLIST, /* TOT List */
  COLUMN_GROUP, /* group */
  COLUMN_SENT, /* sent? */
  EDITOR_N_COLUMNS
};

typedef struct {
  unsigned int roll; /* class roll */
  char st_name[41]; /* student's name */
  char section[2];   /* section (A/B/C/D) */
  char gender[7];    /* gender (Male/Female) */
  unsigned int tot_list;
  char group[20];
  gboolean sent;     /* sent? */
} Data;

typedef struct {
  char *title; /* title of the column */
  char *type; /* type of the column (text, toggle, gdkpixbuf)? */
  int serial; /* serial number of the column, 0 indexed */
} Header;


/* prototypes */
void load_editor(void);
void editor_add_columns(GtkTreeView *tree, Header *header);
GtkWidget *editor_create_window(void);
GtkWidget *editor_create_tree(void);
GtkTreeModel *editor_create_tree_model(void);
GSList *editor_load_data(const char *infodir, GSList **listp);
void editor_store_data(GtkListStore *store, const GSList *list);
GtkWidget *edit_student_info(const char *filename);
void load_student_info(GtkWidget *window, const char *filename);


/* helper function */
void init_editor_path(void);
void update_editor_path(void);
void initdirname(
    const char *, /* root directory */
    const char *, /* session */
    const char *, /* board */
    const char *, /* college */
    char *buffer); /* buffer to save string to (must be large enough */
void refresh_editor_list(void); /* reloads information list */
void print_data(Data *);

/* callbacks */
void editor_checkbox_toggled_cb(GtkCellRendererToggle *, gchar *, gpointer);
//static void row_edited_cb(GtkCellRendererText *, gchar *, gchar *, gpointer);
void editor_column_clicked_cb(GtkTreeViewColumn *col, gpointer data);
void editor_tree_selection_changed_cb(GtkTreeSelection *, gpointer);
void editor_edit_clicked(GtkButton *, gpointer);
void editor_delete_clicked(GtkButton *, gpointer);
void editor_send_clicked(GtkButton *, gpointer);
void editor_clear_clicked(GtkButton *, gpointer);
void editor_refresh_clicked(GtkButton *, gpointer);
void editor_close_clicked(GtkButton *, gpointer);
void editor_options_clicked(GtkButton *button, gpointer data);

gboolean on_window3_delete_event(GtkWidget *, GdkEvent *, gpointer);

void editor_info_close_clicked(GtkButton *, gpointer);
void editor_info_save_clicked(GtkButton *, gpointer);


#endif      /* _EDITOR_H_ */
