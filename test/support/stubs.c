#include "CException.h"

void exit(int code) {
    Throw(code);
}

void perror(const char *s) {}
