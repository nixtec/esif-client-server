/* liststore.h
 */
#include <gtk/gtk.h>

enum {
  COLUMN_STRING,
  COLUMN_INT,
  COLUMN_BOOLEAN,
  N_COLUMNS
};

typedef struct {
  char *data_string;
  int data_int;
  gboolean data_boolean;
} Data;

typedef struct {
  char *title; /* title of the column */
  char *type; /* type of the column (text, toggle, gdkpixbuf)? */
  int serial; /* serial number of the column, 0 indexed */
} Header;


void add_columns(GtkTreeView *tree, Header *header);
GtkWidget *create_window(void);
GtkWidget *create_tree(void);
GtkTreeModel *create_tree_model(void);
void store_data(GtkListStore *store, Data *data);
void checkbox_toggled_cb(GtkCellRendererToggle *, gchar *, gpointer);
//static void row_edited_cb(GtkCellRendererText *, gchar *, gchar *, gpointer);
void column_clicked_cb(GtkTreeViewColumn *col, gpointer data);
void tree_selection_changed_cb(GtkTreeSelection *, gpointer);

