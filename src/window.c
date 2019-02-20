#include <string.h>
#include <stdint.h>

#include "window.h"
#include "memory.h"

#include "loader.h"
#include "emulator.h"

void init_window(char *file_name) {
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, file_name, NULL);
    
    // register
    window_registers[0] = GTK_LABEL (gtk_builder_get_object (builder, "d0"));
    window_registers[1] = GTK_LABEL (gtk_builder_get_object (builder, "d1"));
    window_registers[2] = GTK_LABEL (gtk_builder_get_object (builder, "d2"));
    window_registers[3] = GTK_LABEL (gtk_builder_get_object (builder, "d3"));
    window_registers[4] = GTK_LABEL (gtk_builder_get_object (builder, "d4"));
    window_registers[5] = GTK_LABEL (gtk_builder_get_object (builder, "d5"));
    window_registers[6] = GTK_LABEL (gtk_builder_get_object (builder, "d6"));
    window_registers[7] = GTK_LABEL (gtk_builder_get_object (builder, "d7"));
 
    window_registers[8] = GTK_LABEL (gtk_builder_get_object (builder, "a0"));
    window_registers[9] = GTK_LABEL (gtk_builder_get_object (builder, "a1"));
    window_registers[10] = GTK_LABEL (gtk_builder_get_object (builder, "a2"));
    window_registers[11] = GTK_LABEL (gtk_builder_get_object (builder, "a3"));
    window_registers[12] = GTK_LABEL (gtk_builder_get_object (builder, "a4"));
    window_registers[13] = GTK_LABEL (gtk_builder_get_object (builder, "a5"));
    window_registers[14] = GTK_LABEL (gtk_builder_get_object (builder, "a6"));

    // flag
    window_status_registers[0] = GTK_LABEL (gtk_builder_get_object (builder, "C"));
    window_status_registers[1] = GTK_LABEL (gtk_builder_get_object (builder, "V"));
    window_status_registers[2] = GTK_LABEL (gtk_builder_get_object (builder, "Z"));
    window_status_registers[3] = GTK_LABEL (gtk_builder_get_object (builder, "N"));
    window_status_registers[4] = GTK_LABEL (gtk_builder_get_object (builder, "X"));
    window_status_registers[5] = GTK_LABEL (gtk_builder_get_object (builder, "S"));

    // sp
    window_registers[15] = GTK_LABEL (gtk_builder_get_object (builder, "SSP"));
    window_registers[16] = GTK_LABEL (gtk_builder_get_object (builder, "USP"));
    window_registers[17] = GTK_LABEL (gtk_builder_get_object (builder, "a7"));
    window_pc = GTK_LABEL (gtk_builder_get_object (builder, "PC"));

    window_memory = GTK_LABEL (gtk_builder_get_object (builder, "memory"));

    // link button
    openfile_window = GTK_FILE_CHOOSER_DIALOG(gtk_builder_get_object(builder,
    "OpenFileHex"));

    window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
 
    gtk_widget_show_all(window); 

    update_window();

    gtk_main();
}

void update_window() {
    unsigned i; 
    char buffer[10];
    
    // window register
    for (i = 0; i < 17; ++i) {
        snprintf(buffer, 10, "$%08x", registers[i]);
        gtk_label_set_text(window_registers[i], buffer);
    }
    // a7
    snprintf(buffer, 10, "$%08x", A(7));
    gtk_label_set_text(window_registers[i], buffer);


    // status register
    for (i = 0; i < 6; ++i) {
        snprintf(buffer, 10, "%u", status_registers[i]);
        gtk_label_set_text(window_status_registers[i], buffer);
    }

    // update pc
    snprintf(buffer, 10, "$%08x", PC);
    gtk_label_set_text(window_pc, buffer);

    // update disassembly
}

void openfile_button() {
    gtk_widget_show(GTK_WIDGET(openfile_window));
}

void loadfile_button() {
    char *filename;

    gtk_widget_hide(GTK_WIDGET(openfile_window));
    
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(openfile_window));
    printf("Start loading the file: %s.\n", filename);

    if (memory == NULL) {
        init_memory();
    }

    load_file(filename);

    init();
    update_window();

    free(filename); 
}

void closefile_button() {
    gtk_widget_hide(GTK_WIDGET(openfile_window));
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}