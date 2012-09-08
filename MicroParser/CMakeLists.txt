include_directories (../Scanner/)

add_library (MicroParser MicroParser.h MicroParser.cpp)
INSTALL(TARGETS MicroParser ARCHIVE DESTINATION lib)
