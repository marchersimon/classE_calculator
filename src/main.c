#include <stdio.h>
#include <math.h>
#include <gtk/gtk.h>

enum value {
	freq, vcc, pwr, q, l1, v0
};

enum resultEnum {
	r, c1, c2, l2
};

void calculate(GtkWidget *widget, GtkWidget **entries[]) {
	g_print("Calculating\n");

	gdouble value[6];

	for(int i = 0; i < 6; i++) {
		sscanf(gtk_entry_get_text(GTK_ENTRY(entries[0][i])), "%lf", &value[i]);
	}

	gdouble result[4];

	result[r] = (pow( value[vcc] - value[v0], 2) / value[pwr]) * 0.576801 * (1.0000086 - 0.414395 / value[q] - 0.577501 / pow(value[q], 2) + 0.205967 / pow(value[q], 3));
	result[c1] = pow(10, 12) * ((1 / (pow(M_PI, 2) * value[freq] * pow(10, 6) * result[r] * (pow(M_PI, 2) / 4 + 1))) * (0.99866 + 0.91424 / value[q] - 1.03175 / pow(value[q], 2)) + 0.6 / (pow(2*M_PI*value[freq]*pow(10,6),2)*value[l1]));
	result[c2] = pow(10, 12) * (1 / (2*M_PI*value[freq]*pow(10,6)*result[r]) * (1 / (value[q] - 0.104823)) * (1.00121 + 1.01468 / (value[q] - 1.7879)) - (0.2 / (pow(2*M_PI*value[freq]*pow(10,6),2)*value[l1])));
	result[l2] = pow(10, 6) * (value[q]*result[r]/(2*M_PI*value[freq]*pow(10,6)));

	char temp[100];
	sprintf(temp, "%lf", result[r]);
	gtk_entry_set_text(GTK_ENTRY(entries[1][r]), temp);
	sprintf(temp, "%lf", result[c1]);
	gtk_entry_set_text(GTK_ENTRY(entries[1][c1]), temp);
	sprintf(temp, "%lf", result[c2]);
	gtk_entry_set_text(GTK_ENTRY(entries[1][c2]), temp);
	sprintf(temp, "%lf", result[l2]);
	gtk_entry_set_text(GTK_ENTRY(entries[1][l2]), temp);
}

void initValues (GtkWidget **entries[]) {
	gtk_entry_set_text(GTK_ENTRY(entries[0][0]), "4");
	gtk_entry_set_text(GTK_ENTRY(entries[0][1]), "50");
	gtk_entry_set_text(GTK_ENTRY(entries[0][2]), "50");
	gtk_entry_set_text(GTK_ENTRY(entries[0][3]), "4");
	gtk_entry_set_text(GTK_ENTRY(entries[0][4]), "270");
	gtk_entry_set_text(GTK_ENTRY(entries[0][5]), "0");
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
	g_print("Application closed by user\n");
	gtk_main_quit();
	return FALSE;
}

void displayHelp() {
	char helpMessage[] = 
		"Usage: classE [options]\n"
		"\n"
		"This is a simple programm to calculate component values for class E power amplifiers.\n"
		"\n"
		"Options:\n"
		"\t-h, --help\tDisplay this help message.\n"
		"\t--no-init\tStart with empty input fields.\n";
	g_print(helpMessage);
}

int main(int argc, char *argv[] )
{
	gboolean init = TRUE;

	if(argc > 1) {
		if(!g_strcmp0(argv[1], "--help") || !g_strcmp0(argv[1], "-h")) {
			displayHelp();
			return 0;
		} else if (!g_strcmp0(argv[1], "--no-init")) {
			init = FALSE;
		} else {
			g_print("unrecognised option '%s'\n\n", argv[1]);
			displayHelp();
			return(1);
		}
	}

    GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *eValue[6];
	GtkWidget *lValue[6];
	GtkWidget *lValueUnit[6];
	GtkWidget *b;
	GtkWidget *eResult[4];
	GtkWidget *lResult[4];
	GtkWidget *lResultUnit[4];
	
	GtkWidget **entries[2] = {eValue, eResult};

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	grid = gtk_grid_new();
	for (int i = 0; i < 6; i++) {
		lValue[i] = gtk_label_new(NULL);
		eValue[i] = gtk_entry_new();
		lValueUnit[i] = gtk_label_new(NULL);
	}
	for(int i = 0; i < 4; i++) {
		lResult[i] = gtk_label_new(NULL);
		eResult[i] = gtk_entry_new();
		lResultUnit[i] = gtk_label_new(NULL);
	}
	
	b = gtk_button_new_with_label("Calculate");

	gtk_label_set_markup(GTK_LABEL(lValue[freq]), "f");
	gtk_label_set_markup(GTK_LABEL(lValue[vcc]), "V<sub>CC</sub>");
	gtk_label_set_markup(GTK_LABEL(lValue[pwr]), "P<sub>tot</sub>");
	gtk_label_set_markup(GTK_LABEL(lValue[q]), "Q<sub>L</sub>");
	gtk_label_set_markup(GTK_LABEL(lValue[l1]), "L<sub>1</sub>");
	gtk_label_set_markup(GTK_LABEL(lValue[v0]), "V<sub>0</sub>");
	
	gtk_label_set_markup(GTK_LABEL(lValueUnit[freq]), "MHz");
	gtk_label_set_markup(GTK_LABEL(lValueUnit[vcc]), "V");
	gtk_label_set_markup(GTK_LABEL(lValueUnit[pwr]), "W");
	gtk_label_set_markup(GTK_LABEL(lValueUnit[q]), "");
	gtk_label_set_markup(GTK_LABEL(lValueUnit[l1]), "&#956;H");
	gtk_label_set_markup(GTK_LABEL(lValueUnit[v0]), "V");

	gtk_label_set_markup(GTK_LABEL(lResult[r]), "R");	
	gtk_label_set_markup(GTK_LABEL(lResult[c1]), "C<sub>1</sub>");	
	gtk_label_set_markup(GTK_LABEL(lResult[c2]), "C<sub>2</sub>");		
	gtk_label_set_markup(GTK_LABEL(lResult[l2]), "L<sub>2</sub>");

	gtk_label_set_markup(GTK_LABEL(lResultUnit[r]), "&#937;");
	gtk_label_set_markup(GTK_LABEL(lResultUnit[c1]), "pF");
	gtk_label_set_markup(GTK_LABEL(lResultUnit[c2]), "pF");
	gtk_label_set_markup(GTK_LABEL(lResultUnit[l2]), "&#956;H");

	for(int i = 0; i < 4; i++) {
		gtk_editable_set_editable(GTK_EDITABLE(eResult[i]), 0);
	}


	for(int i = 0; i < 6; i++) {
		gtk_grid_attach(GTK_GRID(grid), lValue[i], 0, i, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), eValue[i], 1, i, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), lValueUnit[i], 2, i, 1, 1);
	}

	gtk_grid_attach(GTK_GRID(grid), b, 0, 6, 3, 1);

	for(int i = 0; i < 4; i++) {
		gtk_grid_attach(GTK_GRID(grid), lResult[i], 0, 7+i, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), eResult[i], 1, 7+i, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), lResultUnit[i], 2, 7+i, 1, 1);
	}

	g_signal_connect(window, "delete-event", G_CALLBACK (delete_event), NULL);
	g_signal_connect(b, "clicked", G_CALLBACK(calculate), entries);

	gtk_container_add(GTK_CONTAINER(window), grid);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
	
	if(init) initValues(entries);

    gtk_widget_show_all(window);


    gtk_main ();

    return 0;
}