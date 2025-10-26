#include "../../../assets/api.h"
#include "../../../assets/lib.h"

typedef int comp_t(const void* a, const void* b);

void quick_sort(void* array, size_t len, void* const pivot, size_t elem_size, comp_t* comp);

void small_sort(void* array, size_t len, size_t elem_size, comp_t* comp);

void swap(void* a, void* b, size_t size);

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

void quick_sort(void* array, size_t len, void* const pivot, size_t elem_size, comp_t* comp) {
    char* arr = (char*)array;

    if (len == 0 || len == 1) return;
    else if (len <= 3)
        small_sort(arr, len, elem_size, comp);

    swap(arr, arr + ((len - 1) / 2) * elem_size, elem_size);
    memcpy(pivot, arr, elem_size);

    size_t left = 0;
    size_t right = len - 1;
    while (1) {
        while (comp(       arr + left  * elem_size, pivot) < 0) left++;
        while (comp(pivot, arr + right * elem_size)        < 0) right--;
        if (left >= right) break;

        swap(arr + left * elem_size, arr + right * elem_size, elem_size);
        left++;
        right--;
    }

    quick_sort(arr, right + 1, pivot, elem_size, comp);
    quick_sort(arr + (right + 1) * elem_size, len - (right + 1), pivot, elem_size, comp);
}

void small_sort(void* array, size_t len, size_t elem_size, comp_t* comp) {
    char* arr = (char*)array;

    if (len <= 3) {
        if(comp(arr, arr + 1 * elem_size) > 0)
            swap(arr, arr + 1 * elem_size, elem_size);
    }
    if (len == 3) {
        if(comp(arr + 1 * elem_size, arr + 2 * elem_size) > 0)
            swap(arr + 1 * elem_size, arr + 2 * elem_size, elem_size);

        if(comp(arr, arr + 1 * elem_size) > 0)
            swap(arr, arr + 1 * elem_size, elem_size);
    }
}

void swap(void* a, void* b, size_t size) {
    char* ca = (char*)a;
    char* cb = (char*)b;

    size_t count = size % sizeof(unsigned int);

    unsigned int c = 0;

    for (size_t i = 0; i < count; i++) {
        memcpy(&c, ca + i * 4, 4);
            memcpy(ca + i * 4, cb + i * 4, 4);
                        memcpy(cb + i * 4, &c, 4);
    }

    if (size & 0x2) {
        static_assert(sizeof(short) == 2);
        short buf = 0;
        memcpy(&buf, ca + count * 4, 2);
              memcpy(ca + count * 4, cb + count * 4, 2);
                              memcpy(cb + count * 4, &buf, 2);
    } else if (size & 0x1) {
        char buf = 0;
        memcpy(&buf, ca + count * 4, 1);
              memcpy(ca + count * 4, cb + count * 4, 1);
                              memcpy(cb + count * 4, &buf, 1);
    }
}

