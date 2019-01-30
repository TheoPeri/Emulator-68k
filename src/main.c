#include "loader.h"
#include <stdio.h>
#include "emulator.h"
#include "stdlib.h"

extern char *memory;

int main() {
    memory = calloc(16777220, sizeof(char));

    int tmp = load_line("S214000000285F245F2212226A000424290008237C29");

    // loader

    // init

    // main loop

    // exit
}
