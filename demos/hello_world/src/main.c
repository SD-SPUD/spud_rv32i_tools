#include "spudkit.h"

int hello_world_main() {
    // initialize spudkit library
    spudkit_init();

    sim_puts("hello world!!!\n");
    uart_puts("\r\nhello world\r\n");

    while(1) {
    }
    return 0;
}
