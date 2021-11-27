/*
 * liststore.c
 */
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "liststore.h"

#define USE_UTIL_FUNC
#include "utils.h"

void gdk_event_handler_func(GdkEvent *event, gpointer data);
void g_destroy_notify_func(gpointer data);

/*
GtkWidget *create_window(void)
{
  GtkWidget *win;

  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(win), "Sample Window");
  g_signal_connect(G_OBJECT(win), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  return win;
}
*/

/* creates tree view and call others to do other works */
GtkWidget *create_tree(void)
{
  GtkWidget *tree;
  GtkTreeModel *model;

  /* data to be stored in the list store */
  Data data[] = {
    { "user0", 0, TRUE },
    { "user1", 1, FALSE },
    { "user2", 2, TRUE },
    { "user3", 3, FALSE },
    { "user4", 4, TRUE },
    { "user5", 5, FALSE },
    { "user6", 6, TRUE },
    { "user7", 7, FALSE },
    { "user8", 8, TRUE },
    { "user9", 9, FALSE },
    { "user10", 10, TRUE },
    { "user11", 11, FALSE },
    { "user12", 12, TRUE },
    { "user13", 13, FALSE },
    { "user14", 14, TRUE },
    { "user15", 15, FALSE },
    { NULL, 0, FALSE }
  };

  /* column header */
  Header col_header[] = {
    { "String", "text" , COLUMN_STRING },
    { "Integer", "text" , COLUMN_INT },
    { "Boolean", "active" , COLUMN_BOOLEAN },
    { NULL, NULL, -1 }
  };


  /* create a new tree view */
  tree = gtk_tree_view_new();
  /* create the list store */
  model = create_tree_model();
  /* set the model (list store) as the model of the tree view */
  gtk_tree_view_set_model(GTK_TREE_VIEW(tree), model);

  /* add column headers to the tree view */
  add_columns(GTK_TREE_VIEW(tree), col_header);

  /* store data to the model */
  store_data(GTK_LIST_STORE(model), data);

  /* Set up the selection handler */
  GtkTreeSelection *select;

  select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
  gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
  g_signal_connect(G_OBJECT(select), "changed",
      G_CALLBACK(tree_selection_changed_cb), NULL);

  return tree;
}

/* add column headers to the tree view */
void add_columns(GtkTreeView *tree, Header *header)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;

  GtkTreeModel *model = gtk_tree_view_get_model(tree);

  if (header == NULL)
    return;

  while (header->serial != -1) {
    if (!strcmp(header->type, "active")) {
      renderer = gtk_cell_renderer_toggle_new();
      g_signal_connect(G_OBJECT(renderer), "toggled",
	  G_CALLBACK(checkbox_toggled_cb), model);
      column =  gtk_tree_view_column_new_with_attributes(header->title,
	  renderer,
	  header->type, header->serial,
	  NULL);
    }
    else {
      renderer = gtk_cell_renderer_text_new();
      g_object_set(G_OBJECT(renderer), "rise", 5, NULL);
//      g_object_set(G_OBJECT(renderer), "editable", TRUE, NULL);
//      g_signal_connect(G_OBJECT(renderer), "edited",
//	  G_CALLBACK(row_edited_cb), model);
//      g_object_set(G_OBJECT(renderer), "foreground", "red", NULL);
      column =  gtk_tree_view_column_new_with_attributes(header->title,
	  renderer,
	  header->type, header->serial,
	  NULL);
      gtk_tree_view_column_set_sort_column_id(column, header->serial);
      /* the above makes the column clickable */
      gtk_tree_view_column_set_resizable(column, TRUE);
    }

    /* the column will be retrieved from this cell later if necessary */
    g_object_set_data(G_OBJECT(renderer), "column",
       GINT_TO_POINTER(header->serial));
    g_signal_connect(G_OBJECT(column), "clicked",
	G_CALLBACK(column_clicked_cb), column);

    /* do something with the column headers */
    gtk_tree_view_column_set_clickable(column, TRUE);
    gtk_tree_view_column_set_reorderable(column, TRUE);
    g_object_set_data(G_OBJECT(column), "colnum",
       GINT_TO_POINTER(header->serial));
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

    /* view holds references of them
     * so we can free them because they are not necessary anymore
     * if you'd allocated them manually, you should free them here
     */
//    free(header->title);
//    free(header->type);
    header++; /* go ahead */
  }
}


GtkTreeModel *create_tree_model(void)
{
  return (GTK_TREE_MODEL(gtk_list_store_new(N_COLUMNS,
      G_TYPE_STRING,
      G_TYPE_INT,
      G_TYPE_BOOLEAN)));

}

/* store data to the list store */
void store_data(GtkListStore *store, Data *data)
{
  GtkTreeIter iter;

  /* uncomment the following if you want to handle all GDK events */
//  gdk_event_handler_set(gdk_event_handler_func, NULL, g_destroy_notify_func);

  while (data->data_string != NULL) {
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
	COLUMN_STRING, data->data_string,
	COLUMN_INT, data->data_int,
	COLUMN_BOOLEAN, data->data_boolean,
	-1);
    data++;
  }
}


/*
int main(int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *tree;

  gtk_init(&argc, &argv);

  window = create_window();
  tree = create_tree();

  gtk_container_add(GTK_CONTAINER(window), tree);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
*/


void checkbox_toggled_cb(GtkCellRendererToggle *cell,
    gchar *path_str,
    gpointer data)
{
  (void) cell;
  printf("*toggled*\n");
  printf("Path : <%s>\n", path_str);
  GtkTreeModel *model = (GtkTreeModel *) data;
  GtkTreeIter iter;
  /* get the path of the cell */
  GtkTreePath *path = gtk_tree_path_new_from_string(path_str);
  gboolean checked;

  /* get toggled iter */
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter, COLUMN_BOOLEAN, &checked, -1);

  /* toggle the value */
  checked ^= 1;

  /* set the new value */
  gtk_list_store_set(GTK_LIST_STORE(model),
      &iter, COLUMN_BOOLEAN, checked, -1);

  /* cleanup */
  gtk_tree_path_free(path);
}

/*
static void row_edited_cb(GtkCellRendererText *cell,
    gchar *path, gchar *new_str, gpointer data)
{
  gint column = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(cell), "column"));
  printf("Column : %d edited\n", column);

  printf("Path : <%s>\n", path);
  printf("New data : %s\n", new_str);
}
*/

void column_clicked_cb(GtkTreeViewColumn *col, gpointer data)
{
  int colnum;
  (void) col;
  (void) data;

  printf("*column clicked*\n");
  colnum = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(col), "colnum"));
  printf("Column Number %d\n", colnum);

}

void
tree_selection_changed_cb(GtkTreeSelection *selection, gpointer data)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *str;

  /* get the event */
  GdkEvent *event;
  event = gtk_get_current_event();

  printf("Event Value : %d\n", event->type);
  printf("Current Event : %s\n", get_event_type(event->type));
  gdk_event_free(event);


  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter, COLUMN_STRING, &str, -1);
    printf("%s selected\n", str);
    g_free(str);
  }
}

void gdk_event_handler_func(GdkEvent *event, gpointer data)
{
  if (event) {
    /* process as you want */
    printf("Event Value : %d\n", event->type);
    printf("Event Type : %s\n", get_event_type(event->type));
    gtk_main_do_event(event); /* pass the event to GTK+ */
  }
}

void g_destroy_notify_func(gpointer data)
{
  printf("Event handler modified\n");
}
