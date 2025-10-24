#include "api.h"
#include "lib.h"

int fibonacci(int n);

int main(int argc, char* argv[]) {
    if (argc != 2)
        return -1;

    return fibonacci(atoi(argv[1]));
}

int fibonacci(int n) {
    if (n <= 1)
        return 0;

    int pref_vals[] = {0, 1};

    for (int i = 2; i < n; i++) {
        int new_val = pref_vals[0] + pref_vals[1];
        pref_vals[0] = pref_vals[1];
        pref_vals[1] = new_val;
    }

    return pref_vals[1];
}

