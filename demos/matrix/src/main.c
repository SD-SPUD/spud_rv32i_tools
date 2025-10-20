#include "spudkit.h"

volatile uint32_t* matrix_data = (volatile uint32_t*)0x96000000;
volatile uint32_t* matrix_addr = (volatile uint32_t*)0x96000004;

void sleepx(int x){
    for(int i = 0; i < x; i++) {
        __asm__ volatile("nop");
    }
}
void clear(){
    for(int i = 0; i < 4096; i++){
       *matrix_addr = i;
       *matrix_data = 0x000000;
    }
}

int main() {
    // initialize the spudkit library
    spudkit_init();
    clear();
    while (1) {
        for(int i = 0; i < 4096; i++){
            clear();
            *matrix_addr = i;
            *matrix_data = 0xFFFFFF;
            sleepx(100000);
        }
    }
    return 0;
}
