include_directories (../include/)

add_library (ACGenerator Generator.h Generator.cpp)
INSTALL(TARGETS ACGenerator ARCHIVE DESTINATION lib)
