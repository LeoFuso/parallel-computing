cmake_minimum_required(VERSION 3.8)
project(matrix_multiplication)

set(CMAKE_C_STANDARD 11)

set(CFLAGS_O0 "-O0")
set(CFLAGS_03 "-O3")


set(SOURCE_FILES main.c)

add_executable(main0 ${SOURCE_FILES})
add_executable(main3 ${SOURCE_FILES})

set_property(TARGET main0 PROPERTY COMPILE_FLAGS ${CFLAGS_O0})
set_property(TARGET main3 PROPERTY COMPILE_FLAGS ${CFLAGS_O3})
