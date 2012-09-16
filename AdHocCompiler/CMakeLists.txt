cmake_minimum_required(VERSION 2.8)
project (ad_hoc_compiler)

set(CMAKE_INSTALL_PREFIX "debug")
message("Building ${CMAKE_PROJECT_NAME}")

add_definitions(-Wall -Werror) # enable all warnings and mark warnings as errors
add_definitions(-O3)  # enable optimizations

add_subdirectory (Token)
add_subdirectory (Scanner)
add_subdirectory (MicroParser)
add_subdirectory (Generator)
add_subdirectory (Test) 
