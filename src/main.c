#include "loader.h"
#include <stdio.h>
#include "emulator.h"
#include "stdlib.h"

extern char *memory;

int main() {
    memory = calloc(16777220, sizeof(char));

    int tmp = copy_raw_data("484452", 3, 0);                                    
                                                                                
    printf("Expect: %hhx, result: %hhx",0x1B, (~(0x06 + tmp) & 0xFF));

    // loader

    // init

    // main loop

    // exit
}
