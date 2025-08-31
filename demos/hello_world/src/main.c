#include <stdint.h>

// Sim stuff
#define CSR_SIM_CTRL_EXIT (0 << 24)
#define CSR_SIM_CTRL_PUTC (1 << 24)

static inline void sim_exit(int exitcode)
{
    unsigned int arg = CSR_SIM_CTRL_EXIT | ((unsigned char)exitcode);
    asm volatile ("csrw dscratch,%0": : "r" (arg));
}

static inline void sim_putc(int ch)
{
    unsigned int arg = CSR_SIM_CTRL_PUTC | (ch & 0xFF);
    asm volatile ("csrw dscratch,%0": : "r" (arg));
}

void str_put(const char* str){
  for(unsigned int i=0; str[i] != '\0'; i++) {
    sim_putc(str[i]);
  }
}

int main() {
    str_put("hello world!!!\n");
    return 0;
}

