include_directories (../include/)

add_library (Generator Generator.h Generator.cpp)
INSTALL(TARGETS Generator ARCHIVE DESTINATION lib)
