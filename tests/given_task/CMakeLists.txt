cmake_minimum_required(VERSION 3.16)

add_test(
    NAME GivenTests
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}/riscv-interpreter-task
    ${CMAKE_MAKE_PROGRAM} -f Makefile test INTERPRETER=${CMAKE_BINARY_DIR}/${PROJECT_NAME}
)

set_property(
    TEST GivenTests
    PROPERTY FAIL_REGULAR_EXPRESSION "FAIL: test_echo|FAIL: test_exit_code_argv|FAIL: test_add|FAIL: test_sub"
)

