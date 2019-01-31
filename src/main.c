#include <stdio.h>

#include "loader.h"
#include "emulator.h"
#include "stdlib.h"

extern char *memory;

int main() {
    init();

    load_file("test.hex");

    shutdown();
}
