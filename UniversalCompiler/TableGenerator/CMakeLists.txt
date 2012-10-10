include_directories (../include/)

add_library (UCTableGenerator TableGenerator.h TableGenerator.cpp)
INSTALL(TARGETS UCTableGenerator ARCHIVE DESTINATION lib)
