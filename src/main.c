#include "stdint.h"

#include "window.h"
#include "emulator.h"

int main() {
    // init the memory
	printf("################# Initializing the emulator ################\n");
    init_memory();

    // init gtk
	printf("######################## Loading GTK #######################\n");
    gtk_init(NULL, NULL);

    // lauch the main win
	printf("################### Initializing interface #################\n");
    init_window("ressources/nouv68k.glade");

	printf("########################### Ready #########################\n");
    gtk_main();

    // free
	printf("################ Shutting down the emulator ###############\n");
    shutdown_emulator();

    return 0;
}
