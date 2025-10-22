cmake_minimum_required(VERSION 3.16)

set(END2END_TEST_LIST
    rv32i/fibonacci
)

set(TASK_TEST_LIST
    echo/runner.sh
    exit_code_argv/runner.sh
    rv32i/test_add
    rv32i/test_sub
)

set(INTERPRETER ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME})

# === End to end tests ===

set(END2END_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/end_to_end)

foreach(test IN LISTS END2END_TEST_LIST)
    add_test(NAME end2end_test_build_${test} COMMAND
        ${CMAKE_COMMAND} -E chdir ${END2END_TEST_DIR}
            ${CMAKE_MAKE_PROGRAM} -f Makefile ${test}
    )

    set_tests_properties(end2end_test_build_${test} PROPERTIES
        FIXTURES_SETUP end2end_test_${test}
    )

    add_test(NAME end2end_test_${test} COMMAND
        ${CMAKE_COMMAND} -E chdir ${END2END_TEST_DIR}
            python3 test_runner.py --interpreter=${INTERPRETER} ${test}
    )
endforeach()

# === TASK tests ===

add_test(
    NAME task_tests_setup
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}/riscv-interpreter-task/
        ${CMAKE_MAKE_PROGRAM} -f Makefile all
)

set_tests_properties(task_tests_setup PROPERTIES
    FIXTURES_SETUP ${TASK_TEST_LIST}
    FIXTURES_REQUIRED
)

set(TASK_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/riscv-interpreter-task/bin)

foreach(test IN LISTS TASK_TEST_LIST)
    get_filename_component(exec_name ${test} NAME)

    if(exec_name STREQUAL "runner.sh")
        get_filename_component(runner_dir ${TASK_TEST_DIR}/${test} DIRECTORY)

        add_test(NAME task_test_${test} COMMAND ${CMAKE_COMMAND} -E chdir ${runner_dir}
            ./runner.sh ${INTERPRETER}
        )
    else()
        add_test(NAME task_test_${test} COMMAND ${CMAKE_COMMAND} -E chdir ${TASK_TEST_DIR}
            ${INTERPRETER} ${TASK_TEST_DIR}/${test}
        )
    endif()
endforeach()

