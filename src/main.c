#include "stdint.h"

#include "window.h"
#include "emulator.h"

int main() {
    // init the memory
    init_memory();

    gtk_init(NULL, NULL);

    init_window("ressources/nouv68k.glade");

    // free
    shutdown_emulator();

    return 0;
}
