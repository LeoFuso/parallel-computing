cmake_minimum_required(VERSION 3.12)
project(open_mp_prime C)

set(CMAKE_C_STANDARD 11)

add_executable(main main.c)

set(CMAKE_C_FLAGS "-O3 -march=native")
target_link_libraries(main m)

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif ()

target_link_libraries(main ${CMAKE_C_FLAGS})

