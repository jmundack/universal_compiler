cmake_minimum_required(VERSION 2.8)

set(CMAKE_INSTALL_PREFIX "debug")
add_definitions(-Wall -Werror) # enable all warnings and mark warnings as errors
add_definitions(-O3)  # enable optimizations
add_definitions(-g)  # enable debugging 

add_subdirectory (AdHocCompiler)
add_subdirectory (UniversalCompiler)
