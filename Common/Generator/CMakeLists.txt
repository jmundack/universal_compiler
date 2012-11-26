include_directories (../include/)

add_library (Generator Generator.h Generator.cpp SymbolTable.h SymbolTable.cpp)
INSTALL(TARGETS Generator ARCHIVE DESTINATION lib)
