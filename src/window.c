#define _GNU_SOURCE

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "window.h"
#include "memory.h"

#include "loader.h"
#include "emulator.h"
#include "debug.h"

/**
 * @brief Check if a string is an hex number
 *
 * @param tmp The string to check
 *
 * @return 0 => NOPE || other => OK
 */
int is_hex(const char *tmp) {
    for (; *tmp; ++tmp) {
        if ((*tmp < '0' || *tmp > '9') && (*tmp < 'a' || *tmp > 'f')) {
            return 0;
        }
    }

    return 1;
}

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

    //memory register
    window_memory_registers[0] = GTK_LABEL (gtk_builder_get_object (builder, "a0_memory"));
    window_memory_registers[1] = GTK_LABEL (gtk_builder_get_object (builder, "a1_memory"));
    window_memory_registers[2] = GTK_LABEL (gtk_builder_get_object (builder, "a2_memory"));
    window_memory_registers[3] = GTK_LABEL (gtk_builder_get_object (builder, "a3_memory"));
    window_memory_registers[4] = GTK_LABEL (gtk_builder_get_object (builder, "a4_memory"));
    window_memory_registers[5] = GTK_LABEL (gtk_builder_get_object (builder, "a5_memory"));
    window_memory_registers[6] = GTK_LABEL (gtk_builder_get_object (builder, "a6_memory"));
    window_memory_registers[7] = GTK_LABEL (gtk_builder_get_object (builder, "a7_memory"));

    //memory string register
    window_memory_str_registers[0] = GTK_LABEL (gtk_builder_get_object (builder, "a0_memory_str"));
    window_memory_str_registers[1] = GTK_LABEL (gtk_builder_get_object (builder, "a1_memory_str"));
    window_memory_str_registers[2] = GTK_LABEL (gtk_builder_get_object (builder, "a2_memory_str"));
    window_memory_str_registers[3] = GTK_LABEL (gtk_builder_get_object (builder, "a3_memory_str"));
    window_memory_str_registers[4] = GTK_LABEL (gtk_builder_get_object (builder, "a4_memory_str"));

    //memory string register
    window_memory_str_registers[0] = GTK_LABEL (gtk_builder_get_object (builder, "a0_memory_str"));
    window_memory_str_registers[1] = GTK_LABEL (gtk_builder_get_object (builder, "a1_memory_str"));
    window_memory_str_registers[2] = GTK_LABEL (gtk_builder_get_object (builder, "a2_memory_str"));
    window_memory_str_registers[3] = GTK_LABEL (gtk_builder_get_object (builder, "a3_memory_str"));
    window_memory_str_registers[4] = GTK_LABEL (gtk_builder_get_object (builder, "a4_memory_str"));
    window_memory_str_registers[5] = GTK_LABEL (gtk_builder_get_object (builder, "a5_memory_str"));
    window_memory_str_registers[6] = GTK_LABEL (gtk_builder_get_object (builder, "a6_memory_str"));
    window_memory_str_registers[7] = GTK_LABEL (gtk_builder_get_object (builder, "a7_memory_str"));

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

    window_pc = GTK_LABEL(gtk_builder_get_object (builder, "PC"));
    window_memory = GTK_LABEL(gtk_builder_get_object (builder, "memory"));

    // file explorer
    openfile_window = GTK_FILE_CHOOSER_DIALOG(gtk_builder_get_object(builder,
    "OpenFileHex"));

    // label memory
    disassembled_memory_a = GTK_LABEL(gtk_builder_get_object(builder,
    "disassembled_memory_a"));

	disassembled_memory_op = GTK_LABEL(gtk_builder_get_object(builder,
    "disassembled_memory_op"));

	disassembled_memory_o = GTK_LABEL(gtk_builder_get_object(builder,
    "disassembled_memory_o"));

    toggle_disassembled_memory = GTK_CHECK_BUTTON(gtk_builder_get_object(builder,
    "Toggle Disassembled Memory"));

	hex_view = GTK_LABEL(gtk_builder_get_object(builder, "hex_view"));
	memory_view = GTK_WIDGET(gtk_builder_get_object(builder, "memory_view"));

    // memory widget
	scrollbar = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "Adjustement"));
    mem_view_position = GTK_ENTRY(gtk_builder_get_object(builder, "memViewPos"));

    // breakpoint widget
    breakpoint_window = GTK_WIDGET(gtk_builder_get_object(builder, "breakpoints_menu"));
    breakpoints_input = GTK_ENTRY(gtk_builder_get_object(builder, "breakpoints_input"));

    // display section
    gtk_display_buffer = malloc(WIN_HEIGHT * WIN_WIDTH * BYTES_PER_PIXEL);
	display_image = GTK_WIDGET(gtk_builder_get_object(builder, "display_image"));

    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
	display_window = GTK_WIDGET(gtk_builder_get_object(builder, "displayWindow"));

    // connect signal
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
    gtk_widget_show_all(main_window);

    // update win
	update_mem_view();
    update_data_window();
}

/**
 * @brief Called when the window is closed
 */
void on_window_main_destroy()
{
    gtk_main_quit();
}

/**
 * @brief Toggle the display window visibility
 */
void toggle_display_window() {
	if(gtk_widget_is_visible(display_window)) {
        gtk_widget_hide(display_window);
    } else {
		gtk_widget_show(display_window);
		update_display();
	}
}

/**
 * @brief Toggle the breakpoint window visibility
 */
void toggle_breakpoint_window() {
    if (gtk_widget_is_visible(breakpoint_window)) {
        gtk_widget_hide(breakpoint_window);
    } else {
		gtk_widget_show(breakpoint_window);
    }
}

/**
 * @brief Toggle the memory view
 */
void toggle_memory_view() {
	if(gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(toggle_disassembled_memory))) {
        gtk_widget_show(GTK_WIDGET(memory_view));
    } else {
        gtk_widget_hide(GTK_WIDGET(memory_view));
    }
}

/**
 * @brief Update the data window
 */
void update_data_window() {
    unsigned i;
    char buffer[256]; //buffer à verifier

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

    // data string register
    for (i = 0; i < 8; ++i) {
        uint32_tostring(buffer, registers[i]);
        gtk_label_set_text(window_str_registers[i], buffer);
    }

    //address memory register //buffer à verifier
    for (i = 0; i < 8; ++i) {
        snprintf(buffer, 512, " %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X ",
            read_16bit_memory(A(i)), read_16bit_memory(A(i)+2), read_16bit_memory(A(i)+4),
            read_16bit_memory(A(i)+6), read_16bit_memory(A(i)+8), read_16bit_memory(A(i)+10),
            read_16bit_memory(A(i)+12), read_16bit_memory(A(i)+14), read_16bit_memory(A(i)+16),
            read_16bit_memory(A(i)+18), read_16bit_memory(A(i)+20), read_16bit_memory(A(i)+22),
            read_16bit_memory(A(i)+24));

        gtk_label_set_text(window_memory_registers[i], buffer);
    }

    //address string memory register
    for (i = 0; i < 8; ++i) {
        memory_tostring(buffer, (char *)(memory + A(i)), 26);
        gtk_label_set_text(window_memory_str_registers[i], buffer);
    }
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
 * @brief Updates the memory visualization buffer
 */
void update_mem_view() {
	size_t first_line = (size_t)(gtk_adjustment_get_value(scrollbar) / 16);

    if (first_line + 0x19 >= MAX_VALUE) {
        first_line = MAX_VALUE - 31;
    }

	char* tmp = NULL;
	char* result = calloc(1, sizeof(char));
	result = mystrcat(result, "<span font_family='SourceCodePro' font='10'>");

	for(size_t i = 0; i < LINE_COUNT; i++)
	{
		unsigned long mem_pos = LINE_SIZE * (first_line + i);
		if(mem_pos >= MEM_SIZE)
		{
			result = mystrcat(result, "\n");
			continue;
		}

		//Make sure we don't read past the memory size
		size_t bytes = MEM_SIZE - mem_pos;
		if(bytes > LINE_SIZE) bytes = LINE_SIZE;

		//Print the address of the current line
		asprintf(&tmp, "<span font_weight='bold'>$%07lX</span>\t", mem_pos);
		result = mystrcat(result, tmp); free(tmp);

		//Print the hex interpretation
		for(size_t j = 0; j < bytes; j++)
		{
			asprintf(&tmp, "%02X ", memory[mem_pos + j]);
			result = mystrcat(result, tmp); free(tmp);
		}

		result = mystrcat(result, "\t<span color='green'>");

		//Print the string interpretation
		for(size_t j = 0; j < bytes; j++)
		{
			char c = memory[mem_pos + j];

            if (c < 0x20 || c == 0x7f) {
				result = mystrcat(result, ".");
            } else {
                switch(c) {
                    case '"':
				        result = mystrcat(result, "&#60;");
                        break;
                    case '&':
				        result = mystrcat(result, "&#62;");
                        break;
                    case '\'':
				        result = mystrcat(result, "&#34;");
                        break;
                    case '<':
				        result = mystrcat(result, "&#38;");
                        break;
                    case '>':
				        result = mystrcat(result, "&#39;");
                        break;
                    default:
                        // dirty
                        asprintf(&tmp, "%c", memory[mem_pos + j]);
				        result = mystrcat(result, tmp);
                        free(tmp);

                        break;
                }
            }
		}

		result = mystrcat(result, "</span>\n");
	}

	result = mystrcat(result, "</span>");

	gtk_label_set_markup(hex_view, result);

	free(result);
}

/**
 * @brief Update the display window
 */
void update_display() {
    if(gtk_widget_is_visible(display_window)) {
        int i, j;
        uint8_t *video_buffer = memory + 0xFFB500; // begin of the display memory
        uint8_t tmp, *iter;

        iter = gtk_display_buffer;

        for (i = 0; i < WIN_HEIGHT * STRIDE_SIZE; ++i) { // iter on all byte
            for (j = 7; j >= 0; --j) { // iterate on each bit
                tmp = (*video_buffer >> j) & 0x1 ? 255 : 0;

                *iter++ = tmp; // setup rgb
                *iter++ = tmp;
                *iter++ = tmp;
            }

            ++video_buffer;
        }

        GdkPixbuf* pb = gdk_pixbuf_new_from_data(
            gtk_display_buffer,
            GDK_COLORSPACE_RGB,
            0,
            8,
            WIN_WIDTH,
            WIN_HEIGHT,
            WIN_STRIDE,
            NULL,
            NULL
        );

        gtk_image_set_from_pixbuf(GTK_IMAGE(display_image), pb);
    }
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
    // TODO
    dict_clear(break_points);

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
    update_data_window();
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
 * @brief Handler for the apply beakpoint button
 */
void apply_breakpoint_button() {
    const char *tmp = gtk_entry_get_text(breakpoints_input);
    uint32_t address;

    if (is_hex(tmp) && sscanf(tmp, "%x", &address) == 1) {
        togglebreakpoint(address);
        toggle_breakpoint_window();
		update_buffer();
    } else {
        printf("Warning the input ($%s) is not an hexadecimal number.\n", tmp);
    }
}

/**
 * @brief Update the scrollbar position at the value of the input
 */
void update_mem_view_position() {
    const char *tmp = gtk_entry_get_text(mem_view_position);
    uint32_t address;

    if (is_hex(tmp) && sscanf(tmp, "%x", &address) == 1
        && address < MEM_MAX_VALUE) {
        gtk_adjustment_set_value(scrollbar, address);
    } else {
        printf("Warning the input ($%s) is invalid.\n", tmp);
    }

    update_mem_view();
}


/**
 * @brief Start the timer on the press increment
 */
void press_increment_mem_view_position() {
    button_pressed = 1;
    g_timeout_add(100, increment_mem_view_position, 0);
}

/**
 * @brief End the timer on the press increment
 */
void release_increment_mem_view_position() {
    button_pressed = 0;
}

/**
 * @brief Update the mem view pos after the button press up
 */
int increment_mem_view_position() {
    const char *tmp = gtk_entry_get_text(mem_view_position);
    uint32_t address;


    if (is_hex(tmp) && sscanf(tmp, "%x", &address) == 1
        && address < MEM_MAX_VALUE) {
        address += 16;

        if (address < MEM_MAX_VALUE) {
            gtk_adjustment_set_value(scrollbar, address);
        } else {
        goto error;
        }
    } else {
        goto error;
    }

    update_mem_view();

    return button_pressed;

error:
    printf("Warning the value ($%x) is out of bound.\n", address);
    return 0;
}

/**
 * @brief Start the timer on the press decrement
 */
void press_decrement_mem_view_position() {
    button_pressed = 1;
    g_timeout_add(100, decrement_mem_view_position, 0);
}

/**
 * @brief End the timer on the press decrement
 */
void release_decrement_mem_view_position() {
    button_pressed = 0;
}

/**
 * @brief Update the mem view pos after the button press down
 */
int decrement_mem_view_position() {
    const char *tmp = gtk_entry_get_text(mem_view_position);
    uint32_t address;


    if (is_hex(tmp) && sscanf(tmp, "%x", &address) == 1
        && address < MEM_MAX_VALUE) {
        address -= 16;

        if (address < MEM_MAX_VALUE) {
            gtk_adjustment_set_value(scrollbar, address);
        } else {
            goto error;
        }
    } else {
        goto error;
    }

    update_mem_view();

    return button_pressed;

error:
    printf("Warning the input ($%s) is invalid.\n", tmp);
    return 0;
}

/**
 * @brief Update the mem view pos at the value of the scrollbar
 */
void update_mem_view_input() {
    char buffer[7];
    uint32_t tmp = gtk_adjustment_get_value(scrollbar);

    sprintf(buffer, "%x", tmp);

    gtk_entry_set_text(mem_view_position, buffer);

    update_mem_view();
}

/**
 * @brief Compute input press from the main win
 *
 * @param widget unused
 * @param event the event value
 *
 * @param widget unused
 * @param event the event value
 */
int key_event_main(__attribute__((unused))GtkWidget *widget, GdkEventKey *event) {
    unsigned i = 0;

    switch (event->keyval) {
        case GDK_KEY_F2:
			togglebreakpoint(PC);
			update_data_window();
            update_buffer();
            return 1;
			break;
		case GDK_KEY_F11:
            next_instruction();
            update_data_window();
            update_buffer();
            return 1;
            break;
        case GDK_KEY_F9:
            if (gtk_widget_is_visible(display_window)) {
                while (!next_instruction() && !dict_get(break_points, PC)) {
                    // test
                    if (i == 512) {
                        i = 0;

                        update_display();

                        // update win
                        gtk_widget_queue_draw(display_window);
                        while (gtk_events_pending()) {
                            gtk_main_iteration();
                        }
                    }

                    ++i;
                }
            } else {
                while (!next_instruction() && !dict_get(break_points, PC));
            }

            update_data_window();
            update_buffer();
            return 1;
            break;
    }

    return 0;
}

/**
 * @brief Compute input press from the display win
 *
 * @param widget unused
 * @param event the event value
 */
void key_press_event_display(__attribute__((unused))GtkWidget *widget, GdkEventKey *event) {
    switch (event->keyval) {
        case GDK_KEY_space:
            write_8bit_memory(0x420, 0xff);
            break;
        case GDK_KEY_Left:
            write_8bit_memory(0x46f, 0xff);
            break;
        case GDK_KEY_Right:
            write_8bit_memory(0x471, 0xff);
            break;
    }
}

/**
 * @brief Compute input release from the display win
 *
 * @param widget unused
 * @param event the event value
 */
void key_release_event_display(__attribute__((unused))GtkWidget *widget, GdkEventKey *event) {
    switch (event->keyval) {
        case GDK_KEY_space:
            write_8bit_memory(0x420, 0x00);
            break;
        case GDK_KEY_Left:
            write_8bit_memory(0x46f, 0x00);
            break;
        case GDK_KEY_Right:
            write_8bit_memory(0x471, 0x00);
            break;
    }
}
