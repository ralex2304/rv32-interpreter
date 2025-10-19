cmake_minimum_required(VERSION 3.16)

project(rv32-interpreter LANGUAGES CXX)

include(cmake/tools.cmake)

include(cmake/defaults.cmake)

include(cmake/third_party.cmake)

add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rvi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rvi_elf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rvi_state.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (LOGGING)
    add_compile_definitions(LOGGING)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE rvi-defaults magic_enum::magic_enum loguru::loguru)

