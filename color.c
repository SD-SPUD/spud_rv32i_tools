#include <stdio.h>

// Standard foreground colors
#define C_BLACK   "\x1b[30m"
#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define C_WHITE   "\x1b[37m"

// Bold foreground colors
#define CB_BLACK   "\x1b[1;30m"
#define CB_RED     "\x1b[1;31m"
#define CB_GREEN   "\x1b[1;32m"
#define CB_YELLOW  "\x1b[1;33m"
#define CB_BLUE    "\x1b[1;34m"
#define CB_MAGENTA "\x1b[1;35m"
#define CB_CYAN    "\x1b[1;36m"
#define CB_WHITE   "\x1b[1;37m"

#define C_RESET   "\x1b[0m"

int main(void) {
    printf(C_BLACK   "BLACK   "   C_RESET);
    printf(C_RED     "RED     "   C_RESET);
    printf(C_GREEN   "GREEN   "   C_RESET);
    printf(C_YELLOW  "YELLOW  "   C_RESET);
    printf(C_BLUE    "BLUE    "   C_RESET);
    printf(C_MAGENTA "MAGENTA "   C_RESET);
    printf(C_CYAN    "CYAN    "   C_RESET);
    printf(C_WHITE   "WHITE   "   C_RESET);
    printf("\n");

    printf(CB_BLACK   "BOLD BLACK   "   C_RESET);
    printf(CB_RED     "BOLD RED     "   C_RESET);
    printf(CB_GREEN   "BOLD GREEN   "   C_RESET);
    printf(CB_YELLOW  "BOLD YELLOW  "   C_RESET);
    printf(CB_BLUE    "BOLD BLUE    "   C_RESET);
    printf(CB_MAGENTA "BOLD MAGENTA "   C_RESET);
    printf(CB_CYAN    "BOLD CYAN    "   C_RESET);
    printf(CB_WHITE   "BOLD WHITE   "   C_RESET);
    printf("\n");

    return 0;
}

