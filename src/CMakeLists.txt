cmake_minimum_required(VERSION 3.14.0)
project(TCAX CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
#-Wl,--unresolved-symbols=report-all
add_subdirectory(tcaxLib)