#ifndef _WINDOW_H
#define _WINDOW_H

#include <gtk/gtk.h>

// mem win macro
#define MEM_MAX_VALUE 16777216
#define MEM_SIZE (MEM_MAX_VALUE * sizeof(uint8_t))
#define MAX_VALUE (MEM_MAX_VALUE / 16 - 1)
#define LINE_SIZE 16
#define LINE_COUNT 32

// win macro
#define BYTES_PER_PIXEL 3

#define WIN_WIDTH 480
#define WIN_HEIGHT 320

#define STRIDE_SIZE (WIN_WIDTH / 8)
#define WIN_STRIDE (WIN_WIDTH * BYTES_PER_PIXEL)

// global parameter and tool
// char buffer of pixel
uint8_t *gtk_display_buffer;

// builder
GtkBuilder *builder;

GtkWidget *main_window;
GtkWidget *display_window;

// debug win widget
GtkLabel *window_status_registers[6];
GtkLabel *window_registers[18];
GtkLabel *window_str_registers[8];
GtkLabel *window_memory_registers[8];
GtkLabel *window_memory_str_registers[8];

GtkLabel *window_pc;
GtkLabel *window_memory;

GtkFileChooserDialog *openfile_window;
GtkFileChooserDialog *openfile_window;

GtkCheckButton *toggle_disassembled_memory;
GtkLabel *disassembled_memory_a;
GtkLabel *disassembled_memory_op;
GtkLabel *disassembled_memory_o;
GtkLabel *hex_view;
GtkWidget *memory_view;
GtkWidget *display_image;

GtkEntry *mem_view_position;

GtkWidget *breakpoint_window;
GtkEntry *breakpoints_input;

GtkAdjustment *scrollbar;

int button_pressed;

// function
// manage win
void init_window();
void on_window_main_destroy();

// toogle windows
void toggle_display_window();
void toggle_breakpoint_window();
void toggle_memory_view();

void update_data_window();
void update_buffer();
void update_mem_view();
void update_display();

// file explorer function
void openfile_button();
void loadfile_button();
void closefile_button();

void apply_breakpoint_button();

void update_mem_view_position();

void press_increment_mem_view_position();
void release_increment_mem_view_position();
int increment_mem_view_position();
int decrement_mem_view_position();
void press_decrement_mem_view_position();
void release_decrement_mem_view_position();

void update_mem_view_input();

int key_event_main(GtkWidget *widget, GdkEventKey *event);
void key_press_event_display(GtkWidget *widget, GdkEventKey *event);
void key_release_event_display(GtkWidget *widget, GdkEventKey *event);

#endif
