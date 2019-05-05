#include "stdint.h"

#include "window.h"
#include "emulator.h"

int main() {
    // init the memory
    init_memory();

    // init gtk
    gtk_init(NULL, NULL);

    // lauch the main win
    init_window("ressources/nouv68k.glade");

    gtk_main();

    // free
    shutdown_emulator();

    return 0;
}
