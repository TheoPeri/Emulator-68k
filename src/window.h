#ifndef _WINDOW_H
#define _WINDOW_H

#include <gtk/gtk.h>

// global parameter and tool
GtkBuilder *builder;
GtkWidget *window;

GtkLabel *window_pc;
GtkLabel *window_memory;

GtkLabel *window_status_registers[6];
GtkLabel *window_registers[18];

GtkFileChooserDialog *openfile_window;
GtkFileChooserDialog *openfile_window;

GtkCheckButton *toggle_disassembled_memory;
GtkLabel *disassembled_memory;

void init_window();
void update_window();
void update_buffer();

void openfile_button();
void loadfile_button();
void closefile_button();

void change_memory_view();

void on_window_main_destroy();


gboolean key_event(GtkWidget *widget, GdkEventKey *event);
#endif
