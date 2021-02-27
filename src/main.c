#include <gtk/gtk.h>

void calculate(GtkWidget *widget, GdkEvent *event, gpointer data) {
	g_print("Calculate");
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
	g_print("delete event occured\n");
	gtk_main_quit();
	return FALSE;
}

int main( int   argc,
          char *argv[] )
{
    GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *e1, *e2, *l1, *l2;
	GtkWidget *b;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	grid = gtk_grid_new();
	e1 = gtk_entry_new();
	e2 = gtk_entry_new();
	l1 = gtk_label_new("N 1");
	l2 = gtk_label_new("N 2");
	b = gtk_button_new_with_label("Calculate");

	gtk_grid_attach(GTK_GRID(grid), l1, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), e1, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), l2, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), e2, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), b, 0, 2, 2, 1);

	g_signal_connect (window, "delete-event", G_CALLBACK (delete_event), NULL);

	g_signal_connect(b, "clicked", G_CALLBACK(calculate), NULL);

	gtk_container_add(GTK_CONTAINER(window), grid);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

	gtk_widget_show(l1);
	gtk_widget_show(l2);
	gtk_widget_show(e1);
	gtk_widget_show(e2);
	gtk_widget_show(b);
	gtk_widget_show(grid);
    gtk_widget_show(window);
    
    gtk_main ();

    return 0;
}