include_directories (../include/)

add_library (UCParserDriverWithSemanticStack ParserDriverWithSemanticStack.h ParserDriverWithSemanticStack.cpp)
INSTALL(TARGETS UCParserDriverWithSemanticStack ARCHIVE DESTINATION lib)
