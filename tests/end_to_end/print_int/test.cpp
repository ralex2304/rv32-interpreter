#include "api.h"
#include "lib.h"

int main() {

    int buffer[256] = {};

    long read_num = read(stdin, (char*)buffer, sizeof(buffer));

    for (unsigned int i = 0; i < read_num / sizeof(int); i++)
        print_int(buffer[i]);
}

