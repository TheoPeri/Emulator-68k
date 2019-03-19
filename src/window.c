#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_XLIB_IMPLEMENTATION

#include "nuklear.h"
#include "nuklear_xlib.h"

#define DTIME           20
#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

#include "window.h"
#include "memory.h"

#include "loader.h"
#include "emulator.h"
#include "debug.h"

// NUKLEAR
typedef struct XWindow XWindow;

struct XWindow {
    Display *dpy;
    Window root;
    Visual *vis;
    Colormap cmap;
    XWindowAttributes attr;
    XSetWindowAttributes swa;
    Window win;
    int screen;
    XFont *font;
    unsigned int width;
    unsigned int height;
    Atom wm_delete_window;
};

static void die(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputs("\n", stderr);
    exit(EXIT_FAILURE);
}

static long timestamp(void)
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0) return 0;
    return (long)((long)tv.tv_sec * 1000 + (long)tv.tv_usec/1000);
}

static void sleep_for(long t)
{
    struct timespec req;
    const time_t sec = (int)(t/1000);
    const long ms = t - (sec * 1000);
    req.tv_sec = sec;
    req.tv_nsec = ms * 1000000L;
    while(-1 == nanosleep(&req, &req));
}

void debug_window(struct nk_context *ctx) {
    if (nk_begin(ctx, "Debug", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT),
        NK_WINDOW_SCALABLE|NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE
        |NK_WINDOW_TITLE))
    {
        
    }
    
    nk_end(ctx);
}

int init_nuklear() {
    long dt;
    long started;
    int running = 1;
    XWindow xw;

    struct nk_context *ctx;

    /* X11 */
    memset(&xw, 0, sizeof xw);
    xw.dpy = XOpenDisplay(NULL);

    if (!xw.dpy) {
        die("Could not open a display; perhaps $DISPLAY is not set?");
    }

    xw.root = DefaultRootWindow(xw.dpy);
    xw.screen = XDefaultScreen(xw.dpy);
    xw.vis = XDefaultVisual(xw.dpy, xw.screen);
    xw.cmap = XCreateColormap(xw.dpy,xw.root,xw.vis,AllocNone);

    xw.swa.colormap = xw.cmap;

    xw.swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
        ButtonPress | ButtonReleaseMask| ButtonMotionMask |
        Button1MotionMask | Button3MotionMask | Button4MotionMask | 
        Button5MotionMask| PointerMotionMask | KeymapStateMask;

    xw.win = XCreateWindow(xw.dpy, xw.root, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
        XDefaultDepth(xw.dpy, xw.screen), InputOutput,
        xw.vis, CWEventMask | CWColormap, &xw.swa);

    XStoreName(xw.dpy, xw.win, "Emulator 68K");
    XMapWindow(xw.dpy, xw.win);
    xw.wm_delete_window = XInternAtom(xw.dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(xw.dpy, xw.win, &xw.wm_delete_window, 1);
    XGetWindowAttributes(xw.dpy, xw.win, &xw.attr);
    xw.width = (unsigned int)xw.attr.width;
    xw.height = (unsigned int)xw.attr.height;

    /* GUI */
    xw.font = nk_xfont_create(xw.dpy, "fixed");
    ctx = nk_xlib_init(xw.font, xw.dpy, xw.screen, xw.win, xw.width, xw.height);

    while (running)
    {
        /* Input */
        XEvent evt;
        started = timestamp();
        
        nk_input_begin(ctx);
        while (XPending(xw.dpy)) {
            XNextEvent(xw.dpy, &evt);
            if (evt.type == ClientMessage) {
                goto cleanup;
            }

            if (XFilterEvent(&evt, xw.win)) {
                continue;
            }

            nk_xlib_handle_event(xw.dpy, xw.screen, xw.win, &evt);
        }
        nk_input_end(ctx);
        
        // main debug window
        debug_window(ctx);

        /* GUI */
        if (nk_window_is_hidden(ctx, "Debug")) {
            break;
        }

        /* Draw */
        XClearWindow(xw.dpy, xw.win);
        nk_xlib_render(xw.win, nk_rgb(30,30,30));
        XFlush(xw.dpy);

        /* Timing */
        dt = timestamp() - started;

        if (dt < DTIME)
            sleep_for(DTIME - dt);
    }

cleanup:
    nk_xfont_del(xw.dpy, xw.font);
    nk_xlib_shutdown();
    XUnmapWindow(xw.dpy, xw.win);
    XFreeColormap(xw.dpy, xw.cmap);
    XDestroyWindow(xw.dpy, xw.win);
    XCloseDisplay(xw.dpy);
    return 0;
}

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

    window_memory = GTK_LABEL(gtk_builder_get_object (builder, "memory"));

    // link button
    openfile_window = GTK_FILE_CHOOSER_DIALOG(gtk_builder_get_object(builder,
    "OpenFileHex"));

    disassembled_memory = GTK_LABEL(gtk_builder_get_object(builder,
    "disassembled_memory"));

    toggle_disassembled_memory = GTK_CHECK_BUTTON(gtk_builder_get_object(builder,
    "Toggle Disassembled Memory"));

    // link key
    window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));

    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(window, "key-press-event", G_CALLBACK(key_event), NULL);

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
}

void update_buffer() {
	char *tmp = pretty_print_instruction();
    gtk_label_set_text(disassembled_memory, tmp);

    free(tmp);
}

void openfile_button() {
    gtk_widget_show(GTK_WIDGET(openfile_window));
}

void loadfile_button() {
    char *filename;

    // get the file name
    gtk_widget_hide(GTK_WIDGET(openfile_window));
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(openfile_window));

    printf("=====\n\nLoading: %s.\n", filename);

    // load the file
    load_file(filename);
    free(filename);

    // init the emulator and update the interface
    init();
    update_window();
    update_buffer();

	printf("\n\n=====\n");
}

void closefile_button() {
    gtk_widget_hide(GTK_WIDGET(openfile_window));
}

void change_memory_view() {
	if(gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(toggle_disassembled_memory))) {
        gtk_widget_hide(GTK_WIDGET(disassembled_memory));
    } else {
        gtk_widget_show(GTK_WIDGET(disassembled_memory));
    }
}

gboolean key_event(__attribute__((unused))GtkWidget *widget,
    GdkEventKey *event) {

    if (event->keyval == 65480) {
        next_instruction();
        update_window();
        update_buffer();
    }

    return FALSE;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
