cmake_minimum_required(VERSION 2.8)
project (universal_compiler)

set(CMAKE_INSTALL_PREFIX "debug")
message("********** Building ${CMAKE_PROJECT_NAME} **********")

add_subdirectory (Scanner)
add_subdirectory (Test) 
