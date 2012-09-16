include_directories (../include/)

add_library (MicroParser MicroParser.h MicroParser.cpp)
INSTALL(TARGETS MicroParser ARCHIVE DESTINATION lib)
target_link_libraries(MicroParser Generator)
