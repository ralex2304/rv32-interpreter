#include "api.h"
#include "lib.h"

enum Fclass {
    NEG_INF     = 1 << 0,
    NEG_NORM    = 1 << 1,
    NEG_SUBNORM = 1 << 2,
    NEG_ZERO    = 1 << 3,
    POS_ZERO    = 1 << 4,
    POS_SUBNORM = 1 << 5,
    POS_NORM    = 1 << 6,
    POS_INF     = 1 << 7,
    SIGNAL_NAN  = 1 << 8,
    QUIET_NAN   = 1 << 9,
};

inline unsigned int fclass(float a) {
    unsigned int ret_val = 0;

    asm("fclass.s %0, %1"
        : "=r"(ret_val)
        : "f"(a));

    return ret_val;
}

int main() {
    int error = 0;

    error += fclass( 123.0f) != POS_NORM;
    error += fclass(-123.0f) != NEG_NORM;

    error += fclass( 0.0f) != POS_ZERO;
    error += fclass(-0.0f) != NEG_ZERO;

    error += fclass(0.0f / 0.0f) != QUIET_NAN;

    error += fclass( 1.0f / 0.0f) != POS_INF;
    error += fclass(-1.0f / 0.0f) != NEG_INF;

    error += fclass( 1.4e-45f) != POS_SUBNORM;
    error += fclass(-1.4e-45f) != NEG_SUBNORM;

    return error;
}

