#include "stdint.h"

#include "window.h"

#include "memory.h"
#include "loader.h"
#include "emulator.h"
#include "debug.h"

int main(int argc, char *argv[])
{
    // init the memory
    init_memory();

    gtk_init(NULL, NULL);

    init_window("nouv68k.glade");

    // free
    shutdown_emulator();

    return 0;
}
