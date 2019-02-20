#include "stdint.h"

#include "window.h"

#include "memory.h"
#include "loader.h"
#include "emulator.h"
#include "debug.h"
 
int main(int argc, char *argv[])
{
    // important
    gtk_init(&argc, &argv);

    init_window("nouv68k.glade"); 

/*
    load_file("assembler/load_exemple.hex");


    pretty_print_instruction();

    shutdown();
    */
    return 0;
}
