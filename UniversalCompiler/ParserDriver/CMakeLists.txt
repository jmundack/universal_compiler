include_directories (../include/)

add_library (UCParserDriver ParserDriver.h ParserDriver.cpp)
INSTALL(TARGETS UCParserDriver ARCHIVE DESTINATION lib)
