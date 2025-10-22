#include "api.h"
#include "lib.h"

int factorial(int n);

int main(int argc, char* argv[]) {
    if (argc != 2)
        return -1;

    return factorial(atoi(argv[1]));
}

int factorial(int n) {
    if (n <= 1)
        return 1;

    return factorial(n - 1) * n;
}

