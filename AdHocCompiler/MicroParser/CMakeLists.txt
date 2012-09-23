include_directories (../include/)

add_library (ACMicroParser MicroParser.h MicroParser.cpp)
INSTALL(TARGETS ACMicroParser ARCHIVE DESTINATION lib)
target_link_libraries(ACMicroParser ACGenerator)
