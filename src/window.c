#define _GNU_SOURCE

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "window.h"
#include "memory.h"

#include "loader.h"
#include "emulator.h"
#include "debug.h"

const size_t MEM_SIZE = 16777216 * sizeof(uint8_t);
const size_t LINE_SIZE = 16;
const size_t LINE_COUNT = 38;

/** 
 * @brief Init the graphic interface
 *
 * @param file_name Need the file path for load the interface.
 */
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

    //string_register
    window_str_registers[0] = GTK_LABEL (gtk_builder_get_object (builder, "d0_str"));
    window_str_registers[1] = GTK_LABEL (gtk_builder_get_object (builder, "d1_str"));
    window_str_registers[2] = GTK_LABEL (gtk_builder_get_object (builder, "d2_str"));
    window_str_registers[3] = GTK_LABEL (gtk_builder_get_object (builder, "d3_str"));
    window_str_registers[4] = GTK_LABEL (gtk_builder_get_object (builder, "d4_str"));
    window_str_registers[5] = GTK_LABEL (gtk_builder_get_object (builder, "d5_str"));
    window_str_registers[6] = GTK_LABEL (gtk_builder_get_object (builder, "d6_str"));
    window_str_registers[7] = GTK_LABEL (gtk_builder_get_object (builder, "d7_str"));

    // window_str_registers[8] = GTK_LABEL (gtk_builder_get_object (builder, "a0_str"));
    // window_str_registers[9] = GTK_LABEL (gtk_builder_get_object (builder, "a1_str"));
    // window_str_registers[10] = GTK_LABEL (gtk_builder_get_object (builder, "a2_str"));
    // window_str_registers[11] = GTK_LABEL (gtk_builder_get_object (builder, "a3_str"));
    // window_str_registers[12] = GTK_LABEL (gtk_builder_get_object (builder, "a4_str"));
    // window_str_registers[13] = GTK_LABEL (gtk_builder_get_object (builder, "a5_str"));
    // window_str_registers[14] = GTK_LABEL (gtk_builder_get_object (builder, "a6_str"));
	// window_str_registers[15] = GTK_LABEL (gtk_builder_get_object (builder, "a7_str"));


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

    window_memory = GTK_LABEL(gtk_builder_get_object (builder, "memory"));

    // link button
    openfile_window = GTK_FILE_CHOOSER_DIALOG(gtk_builder_get_object(builder,
    "OpenFileHex"));

    disassembled_memory_a = GTK_LABEL(gtk_builder_get_object(builder,
    "disassembled_memory_a"));

	disassembled_memory_op = GTK_LABEL(gtk_builder_get_object(builder,
    "disassembled_memory_op"));

	disassembled_memory_o = GTK_LABEL(gtk_builder_get_object(builder,
    "disassembled_memory_o"));

    toggle_disassembled_memory = GTK_CHECK_BUTTON(gtk_builder_get_object(builder,
    "Toggle Disassembled Memory"));

	hex_view = GTK_LABEL(gtk_builder_get_object(builder, "hex_view"));

	scrollbar = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "Adjustement"));
	update_mem_view();

    // link key
    window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));

    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(window, "key-press-event", G_CALLBACK(key_event), NULL);

    g_object_unref(builder);

    gtk_widget_show_all(window);

    update_window();

    gtk_main();
}

/**
 * @brief Update the data window
 */
void update_window() {
    unsigned i;
    char buffer[10];

    // window register
    for (i = 0; i < 17; ++i) {
        snprintf(buffer, 10, "%08X", registers[i]);
        gtk_label_set_text(window_registers[i], buffer);
    }

    // a7
    snprintf(buffer, 10, "%08X", A(7));
    gtk_label_set_text(window_registers[i], buffer);

    // status register
    for (i = 0; i < 6; ++i) {
        snprintf(buffer, 10, "%u", status_registers[i]);
        gtk_label_set_text(window_status_registers[i], buffer);
    }

    // update pc
    snprintf(buffer, 10, "%08X", PC);
    gtk_label_set_text(window_pc, buffer);

    // string register
    for (i = 0; i < 8; ++i) {
        uint32_tostring(buffer, registers[i]);
        gtk_label_set_text(window_str_registers[i], buffer);
    }
}

void scrolled_view()
{
	update_mem_view();
}

/**
 * @brief Updates the memory visualization buffer
 */
void update_mem_view()
{
	size_t first_line = (size_t)(gtk_adjustment_get_value(scrollbar) / LINE_SIZE);

	char* tmp = NULL;
	char* result = calloc(1, sizeof(char));
	result = mystrcat(result, "<span font_family='Monospace'>");

	for(size_t i = 0; i < LINE_COUNT; i++)
	{
		unsigned long mem_pos = LINE_SIZE * (first_line + i);
		//if(mem_pos > MEM_SIZE / 2) break;

		//Make sure we don't read past the memory size
		size_t bytes = MEM_SIZE - mem_pos;
		if(bytes > LINE_SIZE) bytes = LINE_SIZE;

		//Print the address of the current line
		asprintf(&tmp, "<span color='gray'>0x%07lx</span>\t", mem_pos);
		result = mystrcat(result, tmp); free(tmp);

		//Print the hex interpretation
		for(size_t j = 0; j < bytes; j++)
		{
			asprintf(&tmp, "%02x ", memory[mem_pos + j]);
			result = mystrcat(result, tmp); free(tmp);
		}

		result = mystrcat(result, "\t<span color='green'>");

		//Print the string interpretation
		for(size_t j = 0; j < bytes; j++)
		{
			char c = memory[mem_pos + j];
			int utf = c >= 32 && c <= 126 && c != '<' && c != '>';
			if(!utf)
				result = mystrcat(result, ".");
			else
			{
				asprintf(&tmp, "%c", memory[mem_pos + j]);
				result = mystrcat(result, tmp); free(tmp);
			}
		}

		result = mystrcat(result, "</span>\n");
	}
	result = mystrcat(result, "</span>");

	gtk_label_set_markup(hex_view, result);

	free(result);
}

/**
 * @brief Update the disassemble buffer
 */
void update_buffer() {
	char* adrrs = NULL;
	char* opcodes = NULL;
	char* operandes = NULL;

	pretty_print_instruction(&adrrs, &opcodes, &operandes);

    gtk_label_set_markup(disassembled_memory_a, adrrs);
	gtk_label_set_markup(disassembled_memory_op, opcodes);
	gtk_label_set_markup(disassembled_memory_o, operandes);

    free(adrrs);
	free(opcodes);
	free(operandes);

	update_mem_view();
}

/**
 * @brief Open the file explorer
 */
void openfile_button() {
    gtk_widget_show(GTK_WIDGET(openfile_window));
}

/**
 * @brief Load the file in the emulator
 */
void loadfile_button() {
    char *filename;

    // get the file name
    gtk_widget_hide(GTK_WIDGET(openfile_window));
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(openfile_window));

    printf("=====\n\nLoading: %s.\n", filename);

    // load the file
    // reset memory
    memset(memory, 0, 16777216 * sizeof(uint8_t));
    load_file(filename);
    free(filename);

    // reset registers and flags
    memset(registers, 0, 17 * sizeof(uint32_t));
    memset(status_registers, 0, 6 * sizeof(uint8_t));

    // init the emulator and update the interface
    init();
    update_window();
    update_buffer();

	printf("\n\n=====\n");
}

/**
 * @brief Hide the file explorer
 */
void closefile_button() {
    gtk_widget_hide(GTK_WIDGET(openfile_window));
}

/**
 * @brief Change the memory view
 */
void change_memory_view() {
	if(gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(toggle_disassembled_memory))) {
        gtk_widget_hide(GTK_WIDGET(hex_view));
    } else {
        gtk_widget_show(GTK_WIDGET(hex_view));
    }
}

gboolean key_event(__attribute__((unused))GtkWidget *widget,
    GdkEventKey *event) {

    switch (event->keyval) {
        case 65480:
            next_instruction();
            update_window();
            update_buffer();
            break;
        case 65478:
            while (!next_instruction());
            update_window();
            update_buffer();
            break;
    }

    return FALSE;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
