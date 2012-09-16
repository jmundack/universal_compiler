include_directories (../include/)

add_library (Scanner Scanner.h Scanner.cpp)
INSTALL(TARGETS Scanner ARCHIVE DESTINATION lib)
