cmake_minimum_required(VERSION 3.12)
project(open_mp_prime C)

set(CMAKE_C_STANDARD 11)

set(THING "-lm")

add_executable(main main.c)

target_link_libraries(main m)

if (OPENMP_FOUND)
    message(STATUS "OPEN MP FOUND")
#    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    target_link_libraries(main ${OpenMP_C_FLAGS})
    else()
    message(STATUS "OPEN MP NOT FOUND")
endif()

