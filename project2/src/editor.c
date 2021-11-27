/*
 * editor.c
 */
#include "includes.h"          /* common includes and defines */
#include "editor.h"
#include "myfuncs.h"           /* verify_xxx(), etc */
#include "support.h"
#include "callbacks.h"

char infodirname[MAXDIRNAME]; /* directory where informations are saved */

static GtkTreeModel *current_model = NULL;  /* pointer to the current model */
static GtkTreeSelection *current_selection = NULL; /* pointer to current selection */


#ifdef ____NO____
void init_editor_path(void)
{
  time_t curtime = time(NULL);
  strftime(session, 5, "%Y", localtime(&curtime)); /* get current year */

  FILE *fp;
  int collegeval;

  fp = fopen(COLLEGEFILE, "r");
  if (!fp) {
    strcpy(error_msg, strerror(errno));
    PRINTF("init_editor_path: %s: %s", COLLEGEFILE, error_msg);
    return;
  }

  fscanf(fp, "%d", &collegeval);
  PRINTF("init_editor_path: college code read : %d", collegeval);
  fclose(fp);

  snprintf(college_code, sizeof(college_code), "%04d", collegeval);
  PRINTF(" => %s\n", college_code);

  update_editor_path();

}

void update_editor_path(void)
{
  initdirname(ROOTDIR, session, BOARD, college_code, infodirname);
  PRINTF("update_editor_path: %s\n", infodirname);

}

/* initialize the information directory name */
/*
 * initdirname("/home/ayub", "2005", "Rajshahi", "0110", buffer);
 */
void initdirname(const char *rootdir, /* without terminating slash */
    const char *session, /* session */
    const char *board,
    const char *college,
    char *buffer)
{

  if (strlen(rootdir) < 1) {
    PRINTF("initdirname: rootdir empty\n");
    return;
  }
  if (strlen(session) < 1) {
    PRINTF("initdirname: session empty\n");
    return;
  }
  if (strlen(board) < 1) {
    PRINTF("initdirname: board empty\n");
    return;
  }
  if (strlen(college) < 1) {
    PRINTF("initdirname: college empty\n");
    return;
  }

  sprintf(buffer, "%s/%s/%s/INSTITUTE_%s/", rootdir, board, session, college);
  PRINTF("initdirname: %s\n", buffer);
}

#endif /* ____NO____ */

void load_editor(void)
{
  GtkWidget *window;

//  init_editor_path();

  window = editor_create_window();
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  gtk_widget_show_all(window);

}


GtkWidget *editor_create_window(void)
{
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new();

  GtkWidget *win;
  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(win), "Information Editor");
  gtk_window_set_default_size(GTK_WINDOW(win), 600, 400);
  g_signal_connect(G_OBJECT(win), "delete_event", /* ignore close */
      G_CALLBACK(gtk_true), NULL);
  g_signal_connect(G_OBJECT(win), "destroy_event",
      G_CALLBACK(gtk_object_destroy), NULL);

  /* set window icon */
  GdkPixbuf *win_icon_pixbuf = create_pixbuf("bug-buddy.png");
  if (win_icon_pixbuf) {
    gtk_window_set_icon(GTK_WINDOW(win), win_icon_pixbuf);
    gdk_pixbuf_unref(win_icon_pixbuf);
  }

  /* store pointer to widgets for use by lookup_widget() */
  GLADE_HOOKUP_OBJECT_NO_REF(win, win, "editor_win");

  GtkWidget *vbox;
  vbox = gtk_vbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(win), vbox);
  GLADE_HOOKUP_OBJECT(win, vbox, "editor_vbox");

  GtkWidget *sw = gtk_scrolled_window_new(NULL, NULL);
  gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
      GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw),
      GTK_SHADOW_ETCHED_IN);


  GtkWidget *tree;
  tree = editor_create_tree();
  gtk_container_add(GTK_CONTAINER(sw), tree);
  gtk_tooltips_set_tip(tooltips, tree, _("Information Editor Window"), NULL);

//  gtk_box_pack_start(GTK_BOX(vbox), tree, TRUE, TRUE, 0);
  GLADE_HOOKUP_OBJECT(win, tree, "editor_tree");

  /*
  GtkWidget *hbox_info; // college_code, session => container
  hbox_info = gtk_hbox_new(FALSE, 2);
  gtk_box_pack_start(GTK_BOX(vbox), hbox_info, FALSE, FALSE, 0);
  GLADE_HOOKUP_OBJECT(win, hbox_info, "editor_hbox_info");


  GtkWidget *college_label;
  college_label = gtk_label_new(
      _("<span foreground=\"#B56B6B\">College Code: </span>"));
  gtk_label_set_use_markup(GTK_LABEL(college_label), TRUE);
  gtk_box_pack_start(GTK_BOX(hbox_info), college_label, FALSE, FALSE, 5);
  GLADE_HOOKUP_OBJECT(win, college_label, "editor_college_label");

  GtkWidget *college_entry;
  college_entry = gtk_entry_new();
  gtk_tooltips_set_tip(tooltips, college_entry, "College Code (4 digits)",
      NULL);
  gtk_entry_set_max_length(GTK_ENTRY(college_entry), 4);
  gtk_widget_set_size_request(college_entry, 50, 20);
  gtk_box_pack_start(GTK_BOX(hbox_info), college_entry, FALSE, FALSE, 5);
  gtk_entry_set_text(GTK_ENTRY(college_entry), all_colcode);
  GLADE_HOOKUP_OBJECT(win, college_entry, "editor_college_entry");

  GtkWidget *session_label;
  session_label = gtk_label_new(
      _("<span foreground=\"#607D9B\">Session: </span>"));
  gtk_label_set_use_markup(GTK_LABEL(session_label), TRUE);
  gtk_box_pack_start(GTK_BOX(hbox_info), session_label, FALSE, FALSE, 5);
  GLADE_HOOKUP_OBJECT(win, session_label, "editor_session_label");

  GtkWidget *session_entry;
  session_entry = gtk_entry_new();
  gtk_tooltips_set_tip(tooltips, session_entry, "Current Session (4 digits)",
      NULL);
  gtk_entry_set_max_length(GTK_ENTRY(session_entry), 4);
  gtk_widget_set_size_request(session_entry, 50, 20);
  gtk_box_pack_start(GTK_BOX(hbox_info), session_entry, FALSE, FALSE, 5);
  gtk_entry_set_text(GTK_ENTRY(session_entry), session);
  GLADE_HOOKUP_OBJECT(win, session_entry, "editor_session_entry");
  */


  GtkWidget *hbox;
  hbox = gtk_hbox_new(TRUE, 2);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  GLADE_HOOKUP_OBJECT(win, hbox, "editor_hbox");


  GtkWidget *button1;
  button1 = gtk_button_new();
  gtk_box_pack_start(GTK_BOX(hbox), button1, TRUE, FALSE, 0);
  g_signal_connect_swapped((gpointer) button1, "clicked",
      G_CALLBACK(editor_edit_clicked),
      GTK_OBJECT(button1));
  gtk_tooltips_set_tip(tooltips, button1, _("Edit Selected Student's Info"),
      NULL);
  GLADE_HOOKUP_OBJECT(win, button1, "editor_button1");

  /* make the button by default grabbed */
  GTK_WIDGET_SET_FLAGS(button1, GTK_CAN_DEFAULT);
  gtk_widget_grab_default(button1);
  /* ******************** */

  GtkWidget *alignment1;
  alignment1 = gtk_alignment_new(0.5, 0.5, 0, 0);
  gtk_container_add(GTK_CONTAINER(button1), alignment1);
  GLADE_HOOKUP_OBJECT(win, alignment1, "editor_alignment1");

  GtkWidget *hbox1;
  hbox1 = gtk_hbox_new(FALSE, 2);
  gtk_container_add(GTK_CONTAINER(alignment1), hbox1);
  GLADE_HOOKUP_OBJECT(win, hbox1, "editor_hbox1");

  GtkWidget *image1;
  image1 = create_pixmap(NULL, "edit.xpm");
  gtk_box_pack_start(GTK_BOX(hbox1), image1, FALSE, FALSE, 0);
  GLADE_HOOKUP_OBJECT(win, image1, "editor_image1");

  GtkWidget *label1;
  label1 = gtk_label_new_with_mnemonic("_Edit");
  gtk_box_pack_start(GTK_BOX(hbox1), label1, FALSE, FALSE, 0);
  GLADE_HOOKUP_OBJECT(win, label1, "editor_label1");

  /*
  GtkWidget *button_delete;
  button_delete = gtk_button_new_from_stock("gtk-delete");
  gtk_box_pack_start(GTK_BOX(hbox), button_delete, TRUE, FALSE, 0);
  g_signal_connect_swapped(G_OBJECT(button_delete), "clicked",
      G_CALLBACK(editor_delete_clicked), NULL);
  gtk_tooltips_set_tip(tooltips, button_delete, _("Delete Selected Information"), NULL);
  GLADE_HOOKUP_OBJECT(win, button_delete, "editor_button_delete");
  */

  GtkWidget *button2;
  button2 = gtk_button_new();
  gtk_box_pack_start(GTK_BOX(hbox), button2, TRUE, FALSE, 0);
  g_signal_connect_swapped((gpointer) button2, "clicked",
      G_CALLBACK(editor_send_clicked),
      GTK_OBJECT(button2));
  gtk_tooltips_set_tip(tooltips, button2, _("Send unsent forms online"),
      NULL);
  GLADE_HOOKUP_OBJECT(win, button2, "editor_button2");

  GtkWidget *alignment2;
  alignment2 = gtk_alignment_new(0.5, 0.5, 0, 0);
  gtk_container_add(GTK_CONTAINER(button2), alignment2);
  GLADE_HOOKUP_OBJECT(win, alignment2, "editor_alignment2");

  GtkWidget *hbox2;
  hbox2 = gtk_hbox_new(FALSE, 2);
  gtk_container_add(GTK_CONTAINER(alignment2), hbox2);
  GLADE_HOOKUP_OBJECT(win, hbox2, "editor_hbox2");

  GtkWidget *image2;
  image2 = create_pixmap(NULL, "send.xpm");
  gtk_box_pack_start(GTK_BOX(hbox2), image2, FALSE, FALSE, 0);
  GLADE_HOOKUP_OBJECT(win, image2, "editor_image2");

  GtkWidget *label2;
  label2 = gtk_label_new_with_mnemonic("_Send");
  gtk_box_pack_start(GTK_BOX(hbox2), label2, FALSE, FALSE, 0);
  GLADE_HOOKUP_OBJECT(win, label2, "editor_label2");


  GtkWidget *button3;
  button3 = gtk_button_new_with_mnemonic("E_ntry Form");
  gtk_box_pack_start(GTK_BOX(hbox), button3, TRUE, FALSE, 0);
  g_signal_connect_swapped((gpointer) button3, "clicked",
      G_CALLBACK(gtk_window_iconify),
      GTK_OBJECT(win));
  gtk_tooltips_set_tip(tooltips, button3, _("Minimize this window"), NULL);
  GLADE_HOOKUP_OBJECT(win, button3, "editor_button3");

  GtkWidget *button5;
  button5 = gtk_button_new_from_stock("gtk-preferences");
  gtk_box_pack_start(GTK_BOX(hbox), button5, TRUE, FALSE, 0);
  g_signal_connect_swapped(G_OBJECT(button5), "clicked",
      G_CALLBACK(editor_options_clicked), NULL);
  gtk_tooltips_set_tip(tooltips, button5, _("Extra Options"), NULL);
  GLADE_HOOKUP_OBJECT(win, button5, "editor_button5");


  return win;
}

/* creates tree view and call others to do other works */
GtkWidget *editor_create_tree(void)
{
  GtkWidget *tree;
  GtkTreeModel *model; /* GtkListStore */
  GSList *list = NULL;

  /* column header */
  Header col_header[] = {
    { "Class Roll", "text" , COLUMN_ROLL }, /* class roll */
    { "Student's Name", "text" , COLUMN_ST_NAME }, /* student's name */
    { "Section", "text" , COLUMN_SECTION }, /* section */
    { "Gender", "text" , COLUMN_GENDER }, /* gender */
    { "Group", "text", COLUMN_GROUP },
    { "TOT List", "text", COLUMN_TOTLIST },
    { "Sent?", "active" , COLUMN_SENT }, /* sent? */
    { NULL, NULL, -1 }
  };

//  strncpy(infodirname, "/home/ayub/Rajshahi/INSTITUTE_0110/2005/", 255);
//  infodirname[MAXDIRNAME-1] = 0; /* for safety */


  /* create a new tree view */
  tree = gtk_tree_view_new();
  /* create the list store */
  model = editor_create_tree_model();
  current_model = model; /* assign global value */
  /* set the model (list store) as the model of the tree view */
  gtk_tree_view_set_model(GTK_TREE_VIEW(tree), model);

  /* add column headers to the tree view */
  editor_add_columns(GTK_TREE_VIEW(tree), col_header);

  /* load data for the model */
  editor_load_data(infodirname, &list);

  /* store data to the model */
  editor_store_data(GTK_LIST_STORE(model), list);
  PRINTF("editor_create_tree: Freeing editor list\n");
  g_slist_free(list); /* we no longer need it */


  /* Set up the selection handler */
  GtkTreeSelection *select;

  select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
  gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
  g_signal_connect(G_OBJECT(select), "changed",
      G_CALLBACK(editor_tree_selection_changed_cb), NULL);

  return tree;
}

/* add column headers to the tree view */
void editor_add_columns(GtkTreeView *tree, Header *header)
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
	  G_CALLBACK(editor_checkbox_toggled_cb), model);
      column =  gtk_tree_view_column_new_with_attributes(header->title,
	  renderer,
	  header->type, header->serial,
	  NULL);
      gtk_tree_view_column_set_sort_column_id(column, header->serial);
      gtk_tree_view_column_set_resizable(column, TRUE);
    }
    else {
      renderer = gtk_cell_renderer_text_new();
      g_object_set(G_OBJECT(renderer), "rise", 5, NULL);
//      g_object_set(G_OBJECT(renderer), "editable", TRUE, NULL);
//      g_signal_connect(G_OBJECT(renderer), "edited",
//	  G_CALLBACK(editor_row_edited_cb), model);
      g_object_set(G_OBJECT(renderer), "foreground", "#77585E", NULL);
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
	G_CALLBACK(editor_column_clicked_cb), column);

    /* do something with the column headers */
    gtk_tree_view_column_set_clickable(column, TRUE);
//    gtk_tree_view_column_set_reorderable(column, TRUE);
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


GtkTreeModel *editor_create_tree_model(void)
{
  return (GTK_TREE_MODEL(gtk_list_store_new(EDITOR_N_COLUMNS,
      G_TYPE_INT, /* roll */
      G_TYPE_STRING, /* name */
      G_TYPE_STRING, /* section */
      G_TYPE_STRING, /* gender */
      G_TYPE_INT, /* tot list */
      G_TYPE_STRING, /* group */
      G_TYPE_BOOLEAN)));

}

/* loads information data reading 'dir' */
/* caller must supply directory name terminated by a slash '/' */
/* listp may be NULL */
GSList *editor_load_data(const char *infodir, GSList **listp)
{
  GSList *list = NULL;
  Data *data;
  char info_buf[80];
  Data info; /* each iteration's data */
  DIR *dir; /* directory stream pointer */
  struct dirent *file;
  char filename[20];
  FILE *fp;
  char full_filename[MAXDIRNAME+MAXFILENAME];
  int len;

  if (*listp) /* if list already present, free it */
    g_slist_free(*listp);


  if (infodir == NULL) {
    if (listp)
      *listp = NULL;
    return NULL;
  }

  PRINTF("Opening directory\n");
  dir = opendir(infodir);
  if (!dir) {
    strcpy(error_msg, strerror(errno));
    PRINTF("editor_load_data: %s: %s\n", infodir, error_msg);
    if (listp)
      *listp = NULL;
    return NULL;
  }

  filename[MAXFILENAME-1] = '\0'; /* for safety */

  while ((file = readdir(dir))) {
//    if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, ".."))
    if (file->d_name[0] == '.') /* skip hidden, '.', '..' entries */
      continue;
    len = strlen(file->d_name);
    if (len != FILENAMELEN)
      continue; /* skip invalid files */
    if (strcmp(&file->d_name[len-4], ".txt"))
      continue; /* skip non text (non *.txt) files */
    strcpy(full_filename, infodir);
    strcat(full_filename, file->d_name);
//    strncpy(filename, file->d_name, 19);
    PRINTF("FILENAME : %s\n", full_filename);
    /* data to be stored in the list store */
    /* open the file */
    fp = fopen(full_filename, "r");
    if (!fp) {
      PRINTF("fopen: %s: %s\n", full_filename, strerror(errno));
      continue; /* try to open next file */
    }
    /* file opened successfully */
    if (!fgets(info_buf, sizeof(info_buf), fp)) {
      fclose(fp);
      continue;
    }
    memset(&info, 0, sizeof(info));
    PRINTF("INFORMATION READ : %s\n", info_buf);
    sscanf(info_buf, "%[^:]:%d:%[^:]:%d:%[^:]:%d:%[^\n]", info.gender,
	&info.roll, info.section, &info.tot_list, info.group, &info.sent,
       	info.st_name);
    print_data(&info);
    fclose(fp);

    /* now create list */
    data = malloc(sizeof(Data));
    if (data) {
      data->roll = info.roll;
      strncpy(data->section, info.section, 2);
      strncpy(data->gender, info.gender, 7);
      data->sent = (gboolean) info.sent;
      strncpy(data->st_name, info.st_name, 41);
      data->tot_list = info.tot_list;
      strncpy(data->group, info.group, 20);
      list = g_slist_append(list, (gpointer) data);
    }
  }

  PRINTF("Closing directory\n");
  closedir(dir);

  if (listp)
    *listp = list;

  return list;
}


/* store data to the list store */
/* caller should free the list when done */
void editor_store_data(GtkListStore *store, const GSList *list)
{
  GtkTreeIter iter;
  Data *data;
//  GSList *temp;

  /* uncomment the following if you want to handle all GDK events */
//  gdk_event_handler_set(gdk_event_handler_func, NULL, g_destroy_notify_func);

  if (list) { /* not NULL */
    do {
      gtk_list_store_append(store, &iter);
      data = list->data;
      gtk_list_store_set(store, &iter,
	  COLUMN_ROLL, data->roll,
	  COLUMN_ST_NAME, data->st_name,
	  COLUMN_SECTION, data->section,
	  COLUMN_GENDER, data->gender,
	  COLUMN_TOTLIST, data->tot_list,
	  COLUMN_GROUP, data->group,
	  COLUMN_SENT, data->sent,
	  -1);
    } while ((list = g_slist_next(list)));
  }
}



/* ************** CALLBACKS **************** */

void editor_checkbox_toggled_cb(GtkCellRendererToggle *cell,
    gchar *path_str,
    gpointer data)
{
  return; /* do nothing */

//  (void) cell;
//  PRINTF("*toggled*\n");
//  PRINTF("Path : <%s>\n", path_str);
//  GtkTreeModel *model = (GtkTreeModel *) data;
//  GtkTreeIter iter;
  /* get the path of the cell */
//  GtkTreePath *path = gtk_tree_path_new_from_string(path_str);
//  gboolean checked;

  /* get toggled iter */
//  gtk_tree_model_get_iter(model, &iter, path);
//  gtk_tree_model_get(model, &iter, COLUMN_SENT, &checked, -1);

  /* toggle the value */
//  checked ^= 1;

  /* set the new value */
//  gtk_list_store_set(GTK_LIST_STORE(model),
//      &iter, COLUMN_SENT, checked, -1);

  /* cleanup */
//  gtk_tree_path_free(path);
}

/*
void row_edited_cb(GtkCellRendererText *cell,
    gchar *path, gchar *new_str, gpointer data)
{
  gint column = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(cell), "column"));
  PRINTF("Column : %d edited\n", column);

  PRINTF("Path : <%s>\n", path);
  PRINTF("New data : %s\n", new_str);
}
*/

void editor_column_clicked_cb(GtkTreeViewColumn *col, gpointer data)
{
  int colnum;
  (void) col;
  (void) data;

  PRINTF("*column clicked*\n");
  colnum = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(col), "colnum"));
  PRINTF("Column Number %d\n", colnum);

}

void
editor_tree_selection_changed_cb(GtkTreeSelection *selection, gpointer data)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
//  gchar *str;
  int roll;

  /* get the event */
  /*
  GdkEvent *event;
  event = gtk_get_current_event();

  PRINTF("Event Value : %d\n", event->type);
  PRINTF("Current Event : %s\n", get_event_type(event->type));
  gdk_event_free(event);
  */

  current_selection = selection;
  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter, COLUMN_ROLL, &roll, -1);
    PRINTF("%d selected\n", roll);
//    g_free(str);
  }
}

void editor_edit_clicked(GtkButton *button, gpointer data)
{
  GtkTreeSelection *selection;
  GtkTreeModel *model;
  GtkTreeIter iter;

  int roll;
  int tot_list;

  char *section = NULL;
  char *gender = NULL;
  char *group = NULL;
  char full_filename[MAXDIRNAME+MAXFILENAME];
  char filename[MAXFILENAME];

  GtkWidget *edit_info_win;

  PRINTF("editor_edit_clicked\n");


//  model = current_model;
  if (!current_selection) {
    PRINTF("editor_edit_clicked : Not selected\n");
    return;
  }

  selection = current_selection;

  strcpy(full_filename, infodirname);

  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter,
       	COLUMN_ROLL, &roll,
	COLUMN_SECTION, &section,
	COLUMN_GENDER, &gender,
	COLUMN_GROUP, &group,
	COLUMN_TOTLIST, &tot_list,
       	-1);

    /***************************** */
    construct_filename(filename, gender, roll, section, tot_list, group);
//    sprintf(filename, FILEPATTERN, section, gender, roll);
    PRINTF("Edit %s\n", filename);
    strcat(full_filename, filename);

    /* load info for editing */
    edit_info_win = edit_student_info(full_filename);

    if (edit_info_win && !GTK_WIDGET_VISIBLE(edit_info_win))
      gtk_widget_show(edit_info_win);

    /* disable widgets for S. S. C */
    if (is_ssc) {
      GtkWidget *ptr;

      ptr = lookup_widget(edit_info_win, "combo7");
      if (ptr)
	gtk_widget_set_sensitive(ptr, FALSE);

      ptr = lookup_widget(edit_info_win, "entry10");
      if (ptr)
	gtk_widget_set_sensitive(ptr, FALSE);

      ptr = lookup_widget(edit_info_win, "entry11");
      if (ptr)
	gtk_widget_set_sensitive(ptr, FALSE);

      ptr = lookup_widget(edit_info_win, "entry12");
      if (ptr)
	gtk_widget_set_sensitive(ptr, FALSE);

      ptr = lookup_widget(edit_info_win, "entry13");
      if (ptr)
	gtk_widget_set_sensitive(ptr, FALSE);

      ptr = lookup_widget(edit_info_win, "entry14");
      if (ptr)
	gtk_widget_set_sensitive(ptr, FALSE);

    }

    g_free(group);
    g_free(section);
    g_free(gender);
  }
}

void editor_delete_clicked(GtkButton *button, gpointer data)
{
  GtkTreeSelection *selection;
  GtkTreeModel *model;
  GtkTreeIter iter;
  int roll;
  int totlist;
  char *section = NULL;
  char *gender = NULL;
  char *group = NULL;
  char full_filename[MAXDIRNAME+MAXFILENAME];
  char recycle[MAXDIRNAME+MAXFILENAME]; /* recycled name */
  char filename[MAXFILENAME];

  PRINTF("editor_delete_clicked\n");

  strcpy(full_filename, infodirname);
  strcpy(recycle, infodirname);
  strcat(recycle, "recycled/"); /* where to move */
  /* create the directory if not exists */
  if (MKDIR(recycle, 0755)) { /* failed */
    PRINTF("mkdir: %s : %s\n", recycle, strerror(errno));
  }

//  model = current_model;
  if (!current_selection) {
    PRINTF("editor_delete_clicked : Not selected\n");
    return;
  }

  selection = current_selection;

  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter,
       	COLUMN_ROLL, &roll,
	COLUMN_SECTION, &section,
	COLUMN_GENDER, &gender,
	COLUMN_TOTLIST, &totlist,
	COLUMN_GROUP, &group,
       	-1);

    construct_filename(filename, gender, roll, section, totlist, group);

    PRINTF("editor_delete_clicked: Delete %s\n", filename);
    strcat(full_filename, filename); /* filename to move */
    strcat(recycle, filename); /* new path */

    /* remove the file */
//    if (remove(full_filename) != 0) {
//      strcpy(error_msg, strerror(errno));
//      show_message(error_msg);
//    }
//    else { /* removed successfully */
//      strcpy(error_msg, "File removed successfully");
//      show_message(error_msg);
//      refresh_editor_list();
//    }

    /* move it to trash */
    if (rename(full_filename, recycle) != 0) {
      strcpy(error_msg, strerror(errno));
      show_message(error_msg);
    }
    else { /* removed successfully */
//      strcpy(error_msg, "File Removed successfully");
//      show_message(error_msg);
      refresh_editor_list();
    }

    g_free(group);
    g_free(section);
    g_free(gender);
  }
}

void editor_send_clicked(GtkButton *button, gpointer data)
{
  PRINTF("editor_send_clicked\n");

  /*
  GThread *thread;

  PRINTF("editor_send_clicked: ");
  if (!g_thread_supported()) {
    PRINTF("Initializing thread system\n");
    g_thread_init(NULL);
  }

  PRINTF("editor_send_clicked: Creating thread...\n");
  thread = g_thread_create((GThreadFunc) apply_forms, NULL, FALSE, (GError **) NULL);
  */
  apply_forms(NULL);
}

void editor_clear_clicked(GtkButton *button, gpointer data)
{
  PRINTF("editor_clear_clicked\n");
  gtk_list_store_clear(GTK_LIST_STORE(current_model));
}

#ifdef ____NO____
void editor_refresh_clicked(GtkButton *button, gpointer data)
{
  char temp[10];
  GtkWidget *ptr;

  PRINTF("editor_refresh_clicked\n");

  PRINTF("editor_college_entry->lookup ... ");
  ptr = lookup_widget(GTK_WIDGET(button), "editor_college_entry");
  if (ptr) { /* found college code entry */
    PRINTF("found\n");
    strcpy(temp, gtk_entry_get_text(GTK_ENTRY(ptr)));
    if ((strlen(temp) == 4) && !verify_number(temp))
      strcpy(all_colcode, temp); /* update it */
  }

  PRINTF("editor_session_entry->lookup ... ");
  ptr = lookup_widget(GTK_WIDGET(button), "editor_session_entry");
  if (ptr) { /* found college code entry */
    PRINTF("found\n");
    strcpy(temp, gtk_entry_get_text(GTK_ENTRY(ptr)));
    if ((strlen(temp) == 4) && !verify_number(temp))
      strcpy(session, temp); /* update it */
  }

  update_editor_path();

  refresh_editor_list();

}
#endif /* ____NO____ */

void refresh_editor_list(void)
{
  GSList *list = NULL;

  PRINTF("refresh_editor_list: reloading editor list\n");

  gtk_list_store_clear(GTK_LIST_STORE(current_model));
  current_selection = NULL; /* nothing is selected now */

  /* load data again */
  editor_load_data(infodirname, &list);

  /* store data to the model */
  /* frees the 'list'; so don't worry about it */
  editor_store_data(GTK_LIST_STORE(current_model), list);
  PRINTF("refresh_editor_list: Freeing editor list\n");
  g_slist_free(list); /* we no longer need it */


}

void editor_close_clicked(GtkButton *button, gpointer data)
{
  PRINTF("editor_close_clicked\n");
  gtk_main_quit();
}

void print_data(Data *data)
{
  if (!data)
    return;
  PRINTF("SECTION : %s\n", data->section);
  PRINTF("ST_NAME : %s\n", data->st_name);
  PRINTF("GENDER : %s\n", data->gender);
  PRINTF("CLASS ROLL : %04d\n", data->roll);
  PRINTF("SENT? : %d\n", data->sent);
}



/* number of common subject for all */
//unsigned int num_all_common_sub;
/* number of filled subject entries; so that the rest of the entries may be
 * cleared easily
 */

GtkWidget*
edit_student_info(const char *filename)
{
  GtkWidget *window3;
  GdkPixbuf *window3_icon_pixbuf;
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

  /* extra subject code start */
  GtkWidget *frame_ex_1;
  GtkWidget *entry29;
  GtkWidget *label_ex_1;
  GtkWidget *frame_ex_2;
  GtkWidget *entry30;
  GtkWidget *label_ex_2;
  /* extra subject code end */

  GtkWidget *frame39;
  GtkWidget *entry31;
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
  GtkWidget *combo5;
  GList *combo5_items = NULL;
  GtkWidget *combo_entry5;
  GtkWidget *label15;
  GtkWidget *frame15;
  GtkWidget *combo8;
  GList *combo8_items = NULL;
  GtkWidget *combo_entry8;
  GtkWidget *label27;
  GtkWidget *label7;
  GtkWidget *hbuttonbox1;
//  GtkWidget *button2;
  GtkWidget *button3;
  GtkWidget *button4;
  /*
  GtkWidget *alignment1;
  GtkWidget *hbox6;
  GtkWidget *image3;
  GtkWidget *label16;
  */
  GtkTooltips *tooltips;

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


  int idx = 0;
  /* malloc => so that next invocation can't overwrite old pointer */
  GtkWidget **subcodes = NULL;

  /* allocate pointer for subject entries */
  subcodes = (GtkWidget **) malloc((NUM_TOTAL_SUB+1) * sizeof(GtkWidget *));

#ifdef ____NO____
  curtime = time(NULL);
  strftime(year, 5, "%Y", localtime(&curtime)); /* current year */
  yearval = atoi(year);
  yearval -= 15;
  /* **************** */
#endif

  if (!filename) /* filename must be supplied */
    return NULL;

  tooltips = gtk_tooltips_new ();

  window3 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (window3, "window3");
  gtk_container_set_border_width (GTK_CONTAINER (window3), 2);
  gtk_window_set_title (GTK_WINDOW (window3), _("Student's Information Form (SIF)"));
  gtk_window_set_position (GTK_WINDOW (window3), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size (GTK_WINDOW (window3), 800, 560);
  gtk_window_maximize(GTK_WINDOW(window3));

  window3_icon_pixbuf = create_pixbuf ("bug-buddy.png");
  if (window3_icon_pixbuf)
    {
      gtk_window_set_icon (GTK_WINDOW (window3), window3_icon_pixbuf);
      gdk_pixbuf_unref (window3_icon_pixbuf);
    }

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox3, "vbox3");
  gtk_widget_show (vbox3);
  gtk_container_add (GTK_CONTAINER (window3), vbox3);

  hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_widget_set_name (hbox4, "hbox4");
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (vbox3), hbox4, FALSE, TRUE, 0);

  image2 = create_pixmap (window3, "tt.png");
  gtk_widget_set_name (image2, "image2");
  gtk_widget_show (image2);
  gtk_box_pack_start (GTK_BOX (hbox4), image2, TRUE, FALSE, 0);


  /* modified by ayub */

  char label4_str[256];
  int s = (int) strtol(all_session, (char **) NULL, 10);
  s += sess_inc; /* if session is 1998-99, examination is 2000 */

  sprintf(label4_str, "<span foreground=\"#872222\">\n                    S. S. C / H. S. C Examination, %04d\n        Student's Information Form (SIF) for Registration\nBoard of Intermediate and Secondary Education, Rajshahi\n</span>", s);
  label4 = gtk_label_new (_(label4_str));

  /* **************** */


  /*
  label4 = gtk_label_new (_("<span foreground=\"#872222\">\n                      H. S. C Examination, 2005\n      Student's Information Form (SIF) for Registration\nBoard of Intermediate and Secondary Education, Rajshahi\n</span>"));
  */
  gtk_widget_set_name (label4, "label4");
  gtk_widget_show (label4);
  gtk_box_pack_start (GTK_BOX (hbox4), label4, TRUE, TRUE, 0);
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
  GLADE_HOOKUP_OBJECT (window3, photo_frame, "photo_frame");

  /*
  GtkWidget *photo_hint = create_pixmap(window3, "photo-hint.png");
  if (photo_hint) {
    gtk_widget_show(photo_hint);
    gtk_container_add(GTK_CONTAINER(photo_frame), photo_hint);
  }
  */

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
  gtk_entry_set_alignment(GTK_ENTRY(entry8), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (entry8), 4);


  frame3 = gtk_frame_new (NULL);
  gtk_widget_set_name (frame3, "frame3");
  gtk_widget_show (frame3);
  gtk_fixed_put (GTK_FIXED (fixed1), frame3, 136, 96);
  gtk_widget_set_size_request (frame3, 95, 45);

  GtkWidget *zzz_fixed1 = gtk_fixed_new();
  gtk_widget_show(zzz_fixed1);
  gtk_container_add(GTK_CONTAINER(frame3), zzz_fixed1);

  entry5 = gtk_entry_new ();
//  gtk_entry_set_text(GTK_ENTRY(entry5), year); /* fill session */
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
  zzz += 1;
  char zzz_str[5];
  sprintf(zzz_str, "%04d", zzz);
  GtkWidget *dummy_entry5 = gtk_entry_new();

  gtk_entry_set_text(GTK_ENTRY(dummy_entry5), zzz_str);
  gtk_editable_set_editable(GTK_EDITABLE(dummy_entry5), FALSE);
  gtk_widget_set_name (dummy_entry5, "dummy_entry5");
  gtk_widget_show (dummy_entry5);
  gtk_fixed_put(GTK_FIXED(zzz_fixed1), dummy_entry5, 47, 0);
  gtk_widget_set_size_request(dummy_entry5, 40, 24);
//  gtk_container_add (GTK_CONTAINER (frame3), entry5);
  gtk_tooltips_set_tip (tooltips, dummy_entry5, _("Current Education Session (4 digits)"), NULL);
  gtk_entry_set_alignment(GTK_ENTRY(dummy_entry5), 0.5);
  gtk_entry_set_max_length (GTK_ENTRY (dummy_entry5), 4);



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
  combo2_items = g_list_append (combo2_items, (gpointer) _("Rajshahi"));
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
  combo7_items = g_list_append (combo7_items, (gpointer) _("Rajshahi"));
  combo7_items = g_list_append (combo7_items, (gpointer) _("Dhaka"));
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

  /* extra subject code start */
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

  /* extra subject code end */

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
  /* attach data to the entry so that we can get it from callbacks */
  /* free => will be called upon destruction of the window */
  if (subcodes) {
    PRINTF("editor.c : Attaching data to combo_entry1\n");
    g_object_set_data_full(G_OBJECT(combo_entry1), "subentries", subcodes, free);
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

  combo5 = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo5)->popwin),
                     "GladeParentKey", combo5);
  gtk_widget_set_name (combo5, "combo5");
  gtk_widget_show (combo5);
  gtk_box_pack_start (GTK_BOX (hbox5), combo5, TRUE, TRUE, 0);



#ifdef ____NO____
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
#endif /* ____NO____ */


  /*
  combo5_items = g_list_append (combo5_items, (gpointer) _("1990"));
  combo5_items = g_list_append (combo5_items, (gpointer) _("1989"));
  combo5_items = g_list_append (combo5_items, (gpointer) _("1988"));
  combo5_items = g_list_append (combo5_items, (gpointer) _("1987"));
  combo5_items = g_list_append (combo5_items, (gpointer) _("1986"));
  */

  gtk_combo_set_popdown_strings (GTK_COMBO (combo5), combo5_items);
  g_list_free (combo5_items);

  combo_entry5 = GTK_COMBO (combo5)->entry;
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

  /*
  button2 = gtk_button_new_from_stock ("gtk-close");
  gtk_widget_set_name (button2, "button2");
  gtk_widget_show (button2);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button2);
  GTK_WIDGET_SET_FLAGS (button2, GTK_CAN_DEFAULT);
  gtk_tooltips_set_tip (tooltips, button2, _("Close this window"), NULL);
  */

//  button3 = gtk_button_new_from_stock ("gtk-save");
  button3 = gtk_button_new_with_mnemonic("_Save and close");
  gtk_widget_set_name (button3, "button3");
  gtk_widget_show (button3);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button3);
  GTK_WIDGET_SET_FLAGS (button3, GTK_CAN_DEFAULT);
//  gtk_tooltips_set_tip (tooltips, button3, _("Save a copy to local disk"), NULL);
  gtk_tooltips_set_tip (tooltips, button3, _("Save a copy to local disk and close window"), NULL);

  button4 = gtk_button_new_with_mnemonic("_Change Photo");
  gtk_widget_set_name (button4, "button4");
  gtk_widget_show (button4);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button4);
  GTK_WIDGET_SET_FLAGS (button4, GTK_CAN_DEFAULT);
  gtk_tooltips_set_tip (tooltips, button4, _("Change Photo"), NULL);

  /*
  button4 = gtk_button_new ();
  gtk_widget_set_name (button4, "button4");
  gtk_widget_show (button4);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button4);
  GTK_WIDGET_SET_FLAGS (button4, GTK_CAN_DEFAULT);
  gtk_tooltips_set_tip (tooltips, button4, _("Send this form online"), NULL);

  alignment1 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_set_name (alignment1, "alignment1");
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (button4), alignment1);

  hbox6 = gtk_hbox_new (FALSE, 2);
  gtk_widget_set_name (hbox6, "hbox6");
  gtk_widget_show (hbox6);
  gtk_container_add (GTK_CONTAINER (alignment1), hbox6);

  image3 = create_pixmap (window3, "online.png");
  gtk_widget_set_name (image3, "image3");
  gtk_widget_show (image3);
  gtk_box_pack_start (GTK_BOX (hbox6), image3, FALSE, FALSE, 0);

  label16 = gtk_label_new_with_mnemonic (_("S_end"));
  gtk_widget_set_name (label16, "label16");
  gtk_widget_show (label16);
  gtk_box_pack_start (GTK_BOX (hbox6), label16, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label16), GTK_JUSTIFY_LEFT);
  */

  g_signal_connect_swapped ((gpointer) window3, "delete_event",
                            G_CALLBACK (on_window3_delete_event),
                            GTK_OBJECT (window3));
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
  g_signal_connect_swapped ((gpointer) combo_entry5, "changed",
                            G_CALLBACK (on_combo_entry5_changed),
                            GTK_OBJECT (combo_entry5));
  g_signal_connect_swapped ((gpointer) combo_entry7, "changed",
                            G_CALLBACK (on_combo_entry7_changed),
                            GTK_OBJECT (combo_entry7));
  g_signal_connect_swapped ((gpointer) combo_entry1, "changed",
                            G_CALLBACK (on_combo_entry1_changed),
                            GTK_OBJECT (combo_entry1));
  g_signal_connect_swapped ((gpointer) combo_entry8, "changed",
                            G_CALLBACK (on_combo_entry8_changed),
                            GTK_OBJECT (combo_entry8));
  /*
  g_signal_connect_swapped ((gpointer) button2, "clicked",
                            G_CALLBACK (editor_info_close_clicked),
                            GTK_OBJECT (button2));
   */
  g_signal_connect_swapped ((gpointer) button3, "clicked",
                            G_CALLBACK (editor_info_save_clicked),
                            GTK_OBJECT (button3));
  g_signal_connect_swapped ((gpointer) button4, "clicked",
                            G_CALLBACK (on_button4_clicked),
                            GTK_OBJECT (button4));


  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (window3, window3, "window3");
  GLADE_HOOKUP_OBJECT (window3, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (window3, hbox4, "hbox4");
  GLADE_HOOKUP_OBJECT (window3, image2, "image2");
  GLADE_HOOKUP_OBJECT (window3, label4, "label4");
  GLADE_HOOKUP_OBJECT (window3, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (window3, viewport1, "viewport1");
  GLADE_HOOKUP_OBJECT (window3, frame1, "frame1");
  GLADE_HOOKUP_OBJECT (window3, fixed1, "fixed1");
  GLADE_HOOKUP_OBJECT (window3, entry2, "entry2");
  GLADE_HOOKUP_OBJECT (window3, entry3, "entry3");
  GLADE_HOOKUP_OBJECT (window3, entry4, "entry4");
  GLADE_HOOKUP_OBJECT (window3, frame8, "frame8");
  GLADE_HOOKUP_OBJECT (window3, combo6, "combo6");
  GLADE_HOOKUP_OBJECT (window3, combo_entry6, "combo_entry6");
  GLADE_HOOKUP_OBJECT (window3, label18, "label18");
  GLADE_HOOKUP_OBJECT (window3, table1, "table1");
  GLADE_HOOKUP_OBJECT (window3, label19, "label19");
  GLADE_HOOKUP_OBJECT (window3, label20, "label20");
  GLADE_HOOKUP_OBJECT (window3, entry9, "entry9");
  GLADE_HOOKUP_OBJECT (window3, entry8, "entry8");
  GLADE_HOOKUP_OBJECT (window3, frame3, "frame3");
  GLADE_HOOKUP_OBJECT (window3, entry5, "entry5");
  GLADE_HOOKUP_OBJECT (window3, label12, "label12");
  GLADE_HOOKUP_OBJECT (window3, frame5, "frame5");
  GLADE_HOOKUP_OBJECT (window3, entry6, "entry6");
  GLADE_HOOKUP_OBJECT (window3, label14, "label14");
  GLADE_HOOKUP_OBJECT (window3, frame4, "frame4");
  GLADE_HOOKUP_OBJECT (window3, combo2, "combo2");
  GLADE_HOOKUP_OBJECT (window3, combo_entry2, "combo_entry2");
  GLADE_HOOKUP_OBJECT (window3, label13, "label13");
  GLADE_HOOKUP_OBJECT (window3, frame7, "frame7");
  GLADE_HOOKUP_OBJECT (window3, entry7, "entry7");
  GLADE_HOOKUP_OBJECT (window3, label17, "label17");
  GLADE_HOOKUP_OBJECT (window3, frame9, "frame9");
  GLADE_HOOKUP_OBJECT (window3, fixed2, "fixed2");
  GLADE_HOOKUP_OBJECT (window3, frame11, "frame11");
  GLADE_HOOKUP_OBJECT (window3, entry10, "entry10");
  GLADE_HOOKUP_OBJECT (window3, label23, "label23");
  GLADE_HOOKUP_OBJECT (window3, frame12, "frame12");
  GLADE_HOOKUP_OBJECT (window3, entry11, "entry11");
  GLADE_HOOKUP_OBJECT (window3, label24, "label24");
  GLADE_HOOKUP_OBJECT (window3, frame13, "frame13");
  GLADE_HOOKUP_OBJECT (window3, entry12, "entry12");
  GLADE_HOOKUP_OBJECT (window3, label25, "label25");
  GLADE_HOOKUP_OBJECT (window3, frame14, "frame14");
  GLADE_HOOKUP_OBJECT (window3, hbox7, "hbox7");
  GLADE_HOOKUP_OBJECT (window3, entry13, "entry13");
  GLADE_HOOKUP_OBJECT (window3, label53, "label53");
  GLADE_HOOKUP_OBJECT (window3, entry14, "entry14");
  GLADE_HOOKUP_OBJECT (window3, label26, "label26");
  GLADE_HOOKUP_OBJECT (window3, frame10, "frame10");
  GLADE_HOOKUP_OBJECT (window3, combo7, "combo7");
  GLADE_HOOKUP_OBJECT (window3, combo_entry7, "combo_entry7");
  GLADE_HOOKUP_OBJECT (window3, label22, "label22");
  GLADE_HOOKUP_OBJECT (window3, label21, "label21");
  GLADE_HOOKUP_OBJECT (window3, frame16, "frame16");
  GLADE_HOOKUP_OBJECT (window3, combo9, "combo9");
  GLADE_HOOKUP_OBJECT (window3, combo_entry9, "combo_entry9");
  GLADE_HOOKUP_OBJECT (window3, label28, "label28");
  GLADE_HOOKUP_OBJECT (window3, frame17, "frame17");
  GLADE_HOOKUP_OBJECT (window3, table2, "table2");
  GLADE_HOOKUP_OBJECT (window3, frame25, "frame25");
  GLADE_HOOKUP_OBJECT (window3, entry15, "entry15");
  GLADE_HOOKUP_OBJECT (window3, label37, "label37");
  GLADE_HOOKUP_OBJECT (window3, frame26, "frame26");
  GLADE_HOOKUP_OBJECT (window3, entry16, "entry16");
  GLADE_HOOKUP_OBJECT (window3, label38, "label38");
  GLADE_HOOKUP_OBJECT (window3, frame27, "frame27");
  GLADE_HOOKUP_OBJECT (window3, entry17, "entry17");
  GLADE_HOOKUP_OBJECT (window3, label39, "label39");
  GLADE_HOOKUP_OBJECT (window3, frame28, "frame28");
  GLADE_HOOKUP_OBJECT (window3, entry18, "entry18");
  GLADE_HOOKUP_OBJECT (window3, label40, "label40");
  GLADE_HOOKUP_OBJECT (window3, frame29, "frame29");
  GLADE_HOOKUP_OBJECT (window3, entry19, "entry19");
  GLADE_HOOKUP_OBJECT (window3, label41, "label41");
  GLADE_HOOKUP_OBJECT (window3, frame30, "frame30");
  GLADE_HOOKUP_OBJECT (window3, entry20, "entry20");
  GLADE_HOOKUP_OBJECT (window3, label42, "label42");
  GLADE_HOOKUP_OBJECT (window3, frame31, "frame31");
  GLADE_HOOKUP_OBJECT (window3, entry21, "entry21");
  GLADE_HOOKUP_OBJECT (window3, label43, "label43");
  GLADE_HOOKUP_OBJECT (window3, frame32, "frame32");
  GLADE_HOOKUP_OBJECT (window3, entry22, "entry22");
  GLADE_HOOKUP_OBJECT (window3, label44, "label44");
  GLADE_HOOKUP_OBJECT (window3, frame33, "frame33");
  GLADE_HOOKUP_OBJECT (window3, entry23, "entry23");
  GLADE_HOOKUP_OBJECT (window3, label45, "label45");
  GLADE_HOOKUP_OBJECT (window3, frame34, "frame34");
  GLADE_HOOKUP_OBJECT (window3, entry24, "entry24");
  GLADE_HOOKUP_OBJECT (window3, label46, "label46");
  GLADE_HOOKUP_OBJECT (window3, frame35, "frame35");
  GLADE_HOOKUP_OBJECT (window3, entry25, "entry25");
  GLADE_HOOKUP_OBJECT (window3, label47, "label47");
  GLADE_HOOKUP_OBJECT (window3, frame36, "frame36");
  GLADE_HOOKUP_OBJECT (window3, entry26, "entry26");
  GLADE_HOOKUP_OBJECT (window3, label48, "label48");
  GLADE_HOOKUP_OBJECT (window3, frame37, "frame37");
  GLADE_HOOKUP_OBJECT (window3, entry27, "entry27");
  GLADE_HOOKUP_OBJECT (window3, label49, "label49");
  GLADE_HOOKUP_OBJECT (window3, frame38, "frame38");
  GLADE_HOOKUP_OBJECT (window3, entry28, "entry28");
  GLADE_HOOKUP_OBJECT (window3, entry29, "entry29");
  GLADE_HOOKUP_OBJECT (window3, entry30, "entry30");
  GLADE_HOOKUP_OBJECT (window3, label50, "label50");
  GLADE_HOOKUP_OBJECT (window3, frame39, "frame39");
  GLADE_HOOKUP_OBJECT (window3, entry31, "entry31");
  GLADE_HOOKUP_OBJECT (window3, label51, "label51");
  GLADE_HOOKUP_OBJECT (window3, frame40, "frame40");
  GLADE_HOOKUP_OBJECT (window3, button5, "button5");
  GLADE_HOOKUP_OBJECT (window3, label52, "label52");
  GLADE_HOOKUP_OBJECT (window3, label29, "label29");
  GLADE_HOOKUP_OBJECT (window3, label8, "label8");
  GLADE_HOOKUP_OBJECT (window3, label9, "label9");
  GLADE_HOOKUP_OBJECT (window3, label10, "label10");
  GLADE_HOOKUP_OBJECT (window3, frame2, "frame2");
  GLADE_HOOKUP_OBJECT (window3, combo1, "combo1");
  GLADE_HOOKUP_OBJECT (window3, combo_entry1, "combo_entry1");
  GLADE_HOOKUP_OBJECT (window3, label11, "label11");
  GLADE_HOOKUP_OBJECT (window3, frame6, "frame6");
  GLADE_HOOKUP_OBJECT (window3, hbox5, "hbox5");
  GLADE_HOOKUP_OBJECT (window3, combo3, "combo3");
  GLADE_HOOKUP_OBJECT (window3, combo_entry3, "combo_entry3");
  GLADE_HOOKUP_OBJECT (window3, combo4, "combo4");
  GLADE_HOOKUP_OBJECT (window3, combo_entry4, "combo_entry4");
  GLADE_HOOKUP_OBJECT (window3, combo5, "combo5");
  GLADE_HOOKUP_OBJECT (window3, combo_entry5, "combo_entry5");
  GLADE_HOOKUP_OBJECT (window3, label15, "label15");
  GLADE_HOOKUP_OBJECT (window3, frame15, "frame15");
  GLADE_HOOKUP_OBJECT (window3, combo8, "combo8");
  GLADE_HOOKUP_OBJECT (window3, combo_entry8, "combo_entry8");
  GLADE_HOOKUP_OBJECT (window3, label27, "label27");
  GLADE_HOOKUP_OBJECT (window3, label7, "label7");
  GLADE_HOOKUP_OBJECT (window3, hbuttonbox1, "hbuttonbox1");
//  GLADE_HOOKUP_OBJECT (window3, button2, "button2");
  GLADE_HOOKUP_OBJECT (window3, button3, "button3");
  GLADE_HOOKUP_OBJECT (window3, button4, "button4");
//  GLADE_HOOKUP_OBJECT (window3, alignment1, "alignment1");
//  GLADE_HOOKUP_OBJECT (window3, hbox6, "hbox6");
//  GLADE_HOOKUP_OBJECT (window3, image3, "image3");
//  GLADE_HOOKUP_OBJECT (window3, label16, "label16");
  GLADE_HOOKUP_OBJECT_NO_REF (window3, tooltips, "tooltips");

  /* modified by ayub */
  load_student_info(window3, filename); /* load info from filename */
  gtk_widget_grab_focus(entry2); /* focus student's name by default */

  return window3;
}

void load_student_info(GtkWidget *window, const char *filename)
{
  if (!filename)
    return;
  if (!window)
    return;
  
  FILE *fp;
  char name[20];
  char value[80];
  int valint; /* integer equivalent of the value */
  char buffer[MAXLINE];

  char birthday[3];
  char birthmon[4];
  char birthyear[5];

  char sess_start[5]; /* previous session start */
  char sess_end[5];  /* previous session end */

  GtkWidget *ptr;

  fp = fopen(filename, "r");
  if (!fp) {
    PRINTF("fopen: %s: %s\n", filename, strerror(errno));
    return;
  }

  int i = 0;
  fgets(value, sizeof(buffer), fp); /* skip header */
  i++; /* line number starting from 1 */
  fgets(value, sizeof(buffer), fp); /* skip newline */
  i++; /* line number */

  memset(buffer, 0, sizeof(buffer));

  valint = 0;

  while(fgets(buffer, sizeof(buffer), fp)) {
    i++; /* increment line number */
    /* manipulate the data */

    /* clear previous entry */
    memset(name, 0, sizeof(name));
    memset(value, 0, sizeof(value));

    sscanf(buffer, "%s %[^\r\n]", name, value);
    if (i >= 22) /* subject codes start here */
      valint = (int) strtol(value, (char **) NULL, 10);

    PRINTF("Line %d : %s %s\n", i, name, value);
    switch(i) {
      case 3:
	ptr = lookup_widget(window, "entry2");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 4:
	ptr = lookup_widget(window, "entry3");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 5:
	ptr = lookup_widget(window, "entry4");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 6:
	ptr = lookup_widget(window, "combo_entry1");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 7:
	ptr = lookup_widget(window, "entry5");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 8:
	ptr = lookup_widget(window, "combo_entry2");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 9:
	ptr = lookup_widget(window, "entry6");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 10: /* birth date */
	memset(birthday, 0, sizeof(birthday));
	memset(birthmon, 0, sizeof(birthmon));
	memset(birthyear, 0, sizeof(birthyear));
	sscanf(value, "%[^-]-%[^-]-%[^-]", birthday, birthmon, birthyear);
	PRINTF("Birthday : %s\n", birthday);
	PRINTF("Birthmon : %s\n", birthmon);
	PRINTF("Birthyear : %s\n", birthyear);

	ptr = lookup_widget(window, "combo_entry3"); /* day */
	if (ptr)
	  gtk_entry_set_text(GTK_ENTRY(ptr), birthday);
	ptr = lookup_widget(window, "combo_entry4"); /* month */
	if (ptr)
	  gtk_entry_set_text(GTK_ENTRY(ptr), birthmon);
	ptr = lookup_widget(window, "combo_entry5"); /* year */
	if (ptr)
	  gtk_entry_set_text(GTK_ENTRY(ptr), birthyear);
	break;
      case 11:
	ptr = lookup_widget(window, "entry7");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 12:
	ptr = lookup_widget(window, "combo_entry6");
	if (!ptr)
	  break;
	if (strlen(value) < 1) /* sscanf() may skip space */
	  strcpy(value, " ");
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 13:
	ptr = lookup_widget(window, "entry8");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 14:
	ptr = lookup_widget(window, "entry9");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 15:
	ptr = lookup_widget(window, "combo_entry7");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 16:
	ptr = lookup_widget(window, "entry10");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 17:
	ptr = lookup_widget(window, "entry11");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 18:
	ptr = lookup_widget(window, "entry12");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 19: /* previous session */
	memset(sess_start, 0, sizeof(sess_start)); /* clear previous data */
	memset(sess_end, 0, sizeof(sess_end));     /* clear previous data */
	sscanf(value, "%[^\n-]-%[^\n-]", sess_start, sess_end);
	PRINTF("Session start : %s\n", sess_start);
	PRINTF("Session end : %s\n", sess_end);

	ptr = lookup_widget(window, "entry13");
	if (ptr)
	  gtk_entry_set_text(GTK_ENTRY(ptr), sess_start);
	ptr = lookup_widget(window, "entry14");
	if (ptr)
	  gtk_entry_set_text(GTK_ENTRY(ptr), sess_end);
	break;
      case 20:
	ptr = lookup_widget(window, "combo_entry8");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 21:
	ptr = lookup_widget(window, "combo_entry9");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	break;
      case 22:
	ptr = lookup_widget(window, "entry15");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 23:
	ptr = lookup_widget(window, "entry16");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 24:
	ptr = lookup_widget(window, "entry17");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 25:
	ptr = lookup_widget(window, "entry18");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 26:
	ptr = lookup_widget(window, "entry19");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 27:
	ptr = lookup_widget(window, "entry20");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 28:
	ptr = lookup_widget(window, "entry21");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 29:
	ptr = lookup_widget(window, "entry22");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 30:
	ptr = lookup_widget(window, "entry23");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 31:
	ptr = lookup_widget(window, "entry24");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 32:
	ptr = lookup_widget(window, "entry25");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 33:
	ptr = lookup_widget(window, "entry26");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 34:
	ptr = lookup_widget(window, "entry27");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 35:
	ptr = lookup_widget(window, "entry28");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 36:
	ptr = lookup_widget(window, "entry29");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 37:
	ptr = lookup_widget(window, "entry30");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 38:
	ptr = lookup_widget(window, "entry31");
	if (!ptr)
	  break;
	gtk_entry_set_text(GTK_ENTRY(ptr), value);
	if (valint > 100) /* valid subject code */
	  set_sub_tip(ptr, valint);
	break;
      case 39:
	ptr = lookup_widget(window, "photo_frame");
	if (!ptr)
	  break;
	if (strlen(value) <= 0)
	  break;
	GtkWidget *image = gtk_image_new_from_file(value);
	if (image) {
	  gtk_widget_show(image);
	  gtk_container_add(GTK_CONTAINER(ptr), image);
	  char *photo_name = strdup(value);
	  g_object_set_data_full(G_OBJECT(ptr), "photo", photo_name, (GDestroyNotify) free);
	}
	break;


      default: /* no more line allowed */
	break;
    }
  }
  fclose(fp);

}

void editor_info_close_clicked(GtkButton *button, gpointer data)
{
  PRINTF("editor_info_close_clicked\n");
  PRINTF("window3->lookup\n");

  GtkWidget *ptr = lookup_widget(GTK_WIDGET(button), "window3");

  if (ptr)
    gtk_object_destroy(GTK_OBJECT(ptr));
}

/* save and close the window */
void editor_info_save_clicked(GtkButton *button, gpointer data)
{
  GtkWidget *ptr;

  int ok = 1;

  ok = verify_form((gpointer) button, FALSE); /* don't clear entries */
  if ((ok != E_NO_ERROR) || (ok != E_NO_ERROR_FAILED))
    show_error(ok);
  if (ok == E_NO_ERROR) {
    refresh_editor_list();
    ptr = lookup_widget(GTK_WIDGET(button), "window3");
    if (ptr) {
      gtk_widget_hide(ptr);
      gtk_object_destroy(GTK_OBJECT(ptr));
    }
  }
}


gboolean on_window3_delete_event(GtkWidget *widget,
				    GdkEvent *event,
				    gpointer data)
{
  if (GTK_WIDGET_VISIBLE(widget))
    gtk_widget_hide(widget);
  gtk_object_destroy(GTK_OBJECT(widget));
  return FALSE;
}

void editor_options_clicked(GtkButton *button, gpointer data)
{
  create_options_window();
}
