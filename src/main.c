#include <gtk/gtk.h>
#include <stdio.h>   /* sprintf */
#include <stdlib.h>  /* atoi */
#include <string.h>  /* strlen, strcmp */
#include <ctype.h>   /* isdigit */
 
int main(int argc, char *argv[])
{
    GtkBuilder *builder; 
    GtkWidget *window;
 
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "nouv68k.glade", NULL);


    GtkLabel *a0 = GTK_LABEL (gtk_builder_get_object (builder, "a0"));
    GtkLabel *a1 = GTK_LABEL (gtk_builder_get_object (builder, "a1"));
    GtkLabel *a2 = GTK_LABEL (gtk_builder_get_object (builder, "a2"));
    GtkLabel *a3 = GTK_LABEL (gtk_builder_get_object (builder, "a3"));
    GtkLabel *a4 = GTK_LABEL (gtk_builder_get_object (builder, "a4"));
    GtkLabel *a5 = GTK_LABEL (gtk_builder_get_object (builder, "a5"));
    GtkLabel *a6 = GTK_LABEL (gtk_builder_get_object (builder, "a6"));
    GtkLabel *a7 = GTK_LABEL (gtk_builder_get_object (builder, "a7"));
    GtkLabel *d0 = GTK_LABEL (gtk_builder_get_object (builder, "d0"));
    GtkLabel *d1 = GTK_LABEL (gtk_builder_get_object (builder, "d1"));
    GtkLabel *d2 = GTK_LABEL (gtk_builder_get_object (builder, "d2"));
    GtkLabel *d3 = GTK_LABEL (gtk_builder_get_object (builder, "d3"));
    GtkLabel *d4 = GTK_LABEL (gtk_builder_get_object (builder, "d4"));
    GtkLabel *d5 = GTK_LABEL (gtk_builder_get_object (builder, "d5"));
    GtkLabel *d6 = GTK_LABEL (gtk_builder_get_object (builder, "d6"));
    GtkLabel *d7 = GTK_LABEL (gtk_builder_get_object (builder, "d7"));
    GtkLabel *c = GTK_LABEL (gtk_builder_get_object (builder, "C"));
    GtkLabel *v = GTK_LABEL (gtk_builder_get_object (builder, "V"));
    GtkLabel *z = GTK_LABEL (gtk_builder_get_object (builder, "Z"));
    GtkLabel *n = GTK_LABEL (gtk_builder_get_object (builder, "N"));
    GtkLabel *x = GTK_LABEL (gtk_builder_get_object (builder, "X"));
    GtkLabel *s = GTK_LABEL (gtk_builder_get_object (builder, "S"));
    GtkLabel *ssp = GTK_LABEL (gtk_builder_get_object (builder, "SSP"));
    GtkLabel *usp = GTK_LABEL (gtk_builder_get_object (builder, "USP"));
    GtkLabel *pc = GTK_LABEL (gtk_builder_get_object (builder, "PC"));
    GtkLabel *memory = GTK_LABEL (gtk_builder_get_object (builder, "memory"));

    gtk_label_set_text (a0, "$F59809AC");
    gtk_label_set_text (a1, "$00000000");
    gtk_label_set_text (a2, "$00000000");
    gtk_label_set_text (a3, "$00000000");
    gtk_label_set_text (a4, "$00000000");
    gtk_label_set_text (a5, "$00000000");
    gtk_label_set_text (a6, "$00000000");
    gtk_label_set_text (a7, "$00000000");
    gtk_label_set_text (d0, "$214587AF");
    gtk_label_set_text (d1, "$87AFB547");
    gtk_label_set_text (d2, "$00000000");
    gtk_label_set_text (d3, "$00000000");
    gtk_label_set_text (d4, "$00000000");
    gtk_label_set_text (d5, "$00000000");
    gtk_label_set_text (d6, "$00000000");
    gtk_label_set_text (d7, "$00000000");
    gtk_label_set_text (c, "0");
    gtk_label_set_text (n, "1");
    gtk_label_set_text (z, "0");
    gtk_label_set_text (v, "0");
    gtk_label_set_text (x, "1");
    gtk_label_set_text (s, "1");
    gtk_label_set_text (ssp, "$000028B1");
    gtk_label_set_text (usp, "$00000000");
    gtk_label_set_text (pc, "$00002000");
    gtk_label_set_text (memory, "This zone is reserved for the memory information\nThis zone is reserved for the memory information\nThis zone is reserved for the memory information\nThis zone is reserved for the memory information");

 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
 
    gtk_widget_show_all(window); 

    gtk_main();
 
    return 0;
}
 
// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
