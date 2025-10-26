#include "api.h"
#include "lib.h"

const int BUF_SIZE = 1024;

int main() {
    char symbols[BUF_SIZE];

    int size = read(0, symbols, BUF_SIZE);

    if (size < 0 || size > BUF_SIZE)
        return -1;

    for (int i = 1; i < size; i++) {
        for (int j = 0; j < size - i; j++) {
            if (symbols[j] > symbols[j + 1]) {
                char tmp = symbols[j];
                           symbols[j] = symbols[j + 1];
                                        symbols[j + 1] = tmp;
            }
        }
    }

    return (write(1, symbols, size) != size);
}

