include_directories (../include/)

add_library (ACScanner Scanner.h Scanner.cpp)
INSTALL(TARGETS ACScanner ARCHIVE DESTINATION lib)
