project (ad_hoc_compiler)

message("Building ${CMAKE_PROJECT_NAME}")

add_subdirectory (Token)
add_subdirectory (Scanner)
add_subdirectory (MicroParser)
add_subdirectory (Generator)
add_subdirectory (Test) 
