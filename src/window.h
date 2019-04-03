#ifndef _WINDOW_H
#define _WINDOW_H

#include <gtk/gtk.h>

// global parameter and tool
GtkBuilder *builder;
GtkWidget *window;
GtkWidget *console;

GtkLabel *window_pc;
GtkLabel *window_memory;

GtkLabel *window_status_registers[6];
GtkLabel *window_registers[18];
GtkLabel *window_str_registers[8];
GtkLabel *window_memory_registers[8];
GtkLabel *window_memory_str_registers[8];

GtkFileChooserDialog *openfile_window;
GtkFileChooserDialog *openfile_window;

GtkCheckButton *toggle_disassembled_memory;
GtkLabel *disassembled_memory_a;
GtkLabel *disassembled_memory_op;
GtkLabel *disassembled_memory_o;
GtkLabel *hex_view;
GtkWidget *consoleimg;

GtkAdjustment *scrollbar;

void init_window();
void update_window();
void update_buffer();
void update_mem_view();

void openfile_button();
void loadfile_button();
void closefile_button();

void change_mem_view();
void scrolled_view();
void toggle_console();

void on_window_main_destroy();

gboolean key_event(GtkWidget *widget, GdkEventKey *event);
#endif
