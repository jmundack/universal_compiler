include_directories (../include/)

add_library (UCScanner Scanner.h Scanner.cpp)
INSTALL(TARGETS UCScanner ARCHIVE DESTINATION lib)
