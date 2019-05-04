#ifndef _WINDOW_H
#define _WINDOW_H

#include <gtk/gtk.h>

// win macro
#define BYTES_PER_PIXEL 3

#define WIN_WIDTH 480
#define WIN_HEIGHT 320

#define STRIDE_SIZE (WIN_WIDTH / 8)
#define WIN_STRIDE (WIN_WIDTH * BYTES_PER_PIXEL)

// global parameter and tool
uint8_t *gtk_win_buffer;

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
GtkWidget *memory_view;
GtkWidget *consoleimg;

GtkWidget *breakpoints_menu;
GtkEntry *breakpoints_input;

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

void key_event_main(GtkWidget *widget, GdkEventKey *event);
void key_press_event_display(GtkWidget *widget, GdkEventKey *event);
void key_release_event_display(GtkWidget *widget, GdkEventKey *event);
#endif
