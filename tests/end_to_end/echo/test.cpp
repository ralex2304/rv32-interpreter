#include "api.h"

char buffer[254]; //< test .bss segment

int main() {

    int read_num = read(stdin, buffer, sizeof(buffer));

    return write(stdout, buffer, read_num);
}

