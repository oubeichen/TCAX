cmake_minimum_required(VERSION 3.14.0)
project(TCAX CXX)
set(CMAKE_CXX_STANDARD 14)

message("cmake cxx compiler is ${CMAKE_CXX_COMPILER}")
set(CMAKE_CXX_FLAGS "-Wl,--unresolved-symbols=report-all -Wall -Wextra -Werror")
add_subdirectory(tcaxLib)