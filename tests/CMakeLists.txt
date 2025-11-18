cmake_minimum_required(VERSION 3.16)

project(rv32-interpreter-tests)

set(END2END_TEST_LIST
    hello_asm
    echo
    vector_add
    fibonacci
    factorial
    print_int
    bubble_sort
    quick_sort
    float_fma
    fclass
    bit_count
    bit_rotation
)

set(TASK_TEST_LIST
    echo/runner.sh
    exit_code_argv/runner.sh
    rv32i/test_add
    rv32i/test_sub
    rv32im/test_mul
    rv32im/test_div
    rv32imf/test_fadd
    rv32imf/test_fsub
    rv32imf/test_fdiv
    rv32imf/test_fmul
    rv32imf_zbb/test_andn
    rv32imf_zbb/test_shift
    rv32imf_zbb/test_or
    rv32imf_zbb/test_xor
)

if(NOT PROJECT_IS_TOP_LEVEL)
    set(INTERPRETER ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME})
endif()

# === End to end tests ===

set(END2END_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/end_to_end)

set(END2END_TEST_BUILD_LIST "")

foreach(test IN LISTS END2END_TEST_LIST)
    add_custom_command(
        OUTPUT ${END2END_TEST_DIR}/${test}/build
        COMMAND ${CMAKE_COMMAND} -E chdir ${END2END_TEST_DIR}
            python3 test_build.py ${test}
        DEPENDS ${END2END_TEST_DIR}/${test}/
    )

    list(APPEND END2END_TEST_BUILD_LIST ${END2END_TEST_DIR}/${test}/build)

    add_test(NAME end2end_test_${test} COMMAND
        ${CMAKE_COMMAND} -E chdir ${END2END_TEST_DIR}
            python3 test_runner.py --interpreter=${INTERPRETER} ${test}
    )
endforeach()

# === TASK tests ===

set(TASK_TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/riscv-interpreter-task)
set(TASK_TEST_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/riscv-interpreter-task/bin)

add_custom_command(
    OUTPUT ${TASK_TEST_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E chdir ${TASK_TEST_SOURCE_DIR}
        ${CMAKE_MAKE_PROGRAM} -f Makefile all
)

foreach(test IN LISTS TASK_TEST_LIST)
    get_filename_component(exec_name ${test} NAME)

    if(exec_name STREQUAL "runner.sh")
        get_filename_component(runner_dir ${TASK_TEST_BINARY_DIR}/${test} DIRECTORY)

        add_test(NAME task_test_${test} COMMAND ${CMAKE_COMMAND} -E chdir ${runner_dir}
            ./runner.sh ${INTERPRETER}
        )
    else()
        add_test(NAME task_test_${test} COMMAND ${CMAKE_COMMAND} -E chdir ${TASK_TEST_BINARY_DIR}
            ${INTERPRETER} ${TASK_TEST_BINARY_DIR}/${test}
        )
    endif()
endforeach()

# === Tests build ===

set(ALL_TESTS_BUILD_LIST ${END2END_TEST_BUILD_LIST} ${TASK_TEST_BINARY_DIR})

if(PROJECT_IS_TOP_LEVEL OR ALWAYS_BUILD_TESTS)
    add_custom_target(TestsBuild ALL DEPENDS ${ALL_TESTS_BUILD_LIST})
else()
    add_custom_target(TestsBuild DEPENDS ${ALL_TESTS_BUILD_LIST})
endif()

