add_library(rvi-defaults INTERFACE)

target_compile_features(rvi-defaults INTERFACE cxx_std_23)

target_compile_options(rvi-defaults
    INTERFACE
        -fdiagnostics-color=always

        -Wall
        -Wextra
        -Werror=vla
        -Wstrict-aliasing
        -Wsign-promo
        -Wpointer-arith
        -Woverloaded-virtual
        -Wnon-virtual-dtor
        -Wno-missing-field-initializers
        -Wignored-qualifiers
        -Wformat=2
        -Wformat-security
        -Wempty-body
        -Wctor-dtor-privacy
        -Wconversion
        -Wcast-qual
        -Wno-narrowing
        -Wno-old-style-cast
        -Wmain
        -Wswitch-enum
        -Wswitch-default
        -Wfloat-equal
        -Wunused-parameter
        -Wunreachable-code
        -Wmissing-declarations
        -Wmissing-include-dirs
        -Wfloat-equal
        -Wunreachable-code
        -Wmissing-declarations
        -Wmissing-include-dirs

        -fstack-protector-strong
        -fcheck-new
        -fstrict-overflow
        -pedantic

        $<$<CONFIG:Debug>:
            -Og
            -g3
            -ggdb
        >

        $<$<CONFIG:Release>:
            -O2
            -march=native
            -flto
            -DNDEBUG
        >
)

target_link_options(rvi-defaults
    INTERFACE
        $<$<CONFIG:Debug>:
            -Og
            -g3
            -ggdb
        >

        $<$<CONFIG:Release>:
            -O2
            -march=native
            -flto
        >
)

