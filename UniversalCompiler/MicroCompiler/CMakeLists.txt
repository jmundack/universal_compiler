include_directories (../include/)

add_library (UCMicroCompiler MicroCompiler.h MicroCompiler.cpp)
INSTALL(TARGETS UCMicroCompiler ARCHIVE DESTINATION lib)
